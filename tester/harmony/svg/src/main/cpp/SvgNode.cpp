#include "SvgNode.h"
#include <native_drawing/drawing_matrix.h>
#include <native_drawing/drawing_path.h>
#include <regex>
#include <string>
#include "properties/SvgDomType.h"
#include "utils/LinearMap.h"
#include "utils/StringUtils.h"
#include "utils/SvgAttributesParser.h"
#include "utils/Utils.h"
#include "utils/StringUtils.h"
#include "utils/SvgAttributesParser.h"

namespace rnoh {

void SvgNode::InitStyle(const SvgBaseAttribute &attr) {
    InheritAttr(attr);
    OnInitStyle();
    if (passStyle_) {
        for (auto &node : children_) {
            // pass down style only if child inheritStyle_ is true
            node->InitStyle((node->inheritStyle_) ? attributes_ : SvgBaseAttribute());
        }
    }
}

void SvgNode::OnDrawTraversed(OH_Drawing_Canvas *canvas) {
    auto smoothEdge = GetSmoothEdge();
    for (auto &node : children_) {
        if (node && node->drawTraversed_) {
            if (GreatNotEqual(smoothEdge, 0.0f)) {
                node->SetSmoothEdge(smoothEdge);
            }
            node->Draw(canvas);
        }
    }
}

const Rect &SvgNode::GetRootViewBox() const {
    if (!context_) {
//         LOGE("Gradient failed, svgContext is null");
        static Rect empty;
        return empty;
    }
    return context_->GetRootViewBox();
}

void SvgNode::OnClipPath(OH_Drawing_Canvas *canvas) {
    if (!context_) {
        LOG(INFO) << "NO CONTEXT";
        return;
    }
    auto refSvgNode = context_->GetSvgNodeById(hrefClipPath_);
    if (!refSvgNode) {
        return;
    };
    auto *clipPath = refSvgNode->AsPath();
    OH_Drawing_CanvasClipPath(canvas, clipPath, OH_Drawing_CanvasClipOp::INTERSECT, true);
    OH_Drawing_PathDestroy(clipPath);
}

void SvgNode::OnMask(OH_Drawing_Canvas *canvas) {
    if (!context_) {
        LOG(INFO) << "NO CONTEXT";
        return;
    }
    auto refMask = context_->GetSvgNodeById(attributes_.maskId);
    if (!refMask) {
        return;
    };
    refMask->Draw(canvas);
}

void SvgNode::OnTransform(OH_Drawing_Canvas *canvas) {
    const auto &transform = attributes_.transform;
    auto *matrix = OH_Drawing_MatrixCreate();
    OH_Drawing_MatrixSetMatrix(matrix, transform[0], transform[1], transform[2], transform[3], transform[4],
                               transform[5], transform[6], transform[7], transform[8]);
    OH_Drawing_CanvasConcatMatrix(canvas, matrix);
    OH_Drawing_MatrixDestroy(matrix);
}

double SvgNode::ConvertDimensionToPx(const Dimension &value, const Size &viewPort, SvgLengthType type) const {
    switch (value.Unit()) {
    case DimensionUnit::PERCENT: {
        if (type == SvgLengthType::HORIZONTAL) {
            return value.Value() * viewPort.Width();
        }
        if (type == SvgLengthType::VERTICAL) {
            return value.Value() * viewPort.Height();
        }
        if (type == SvgLengthType::OTHER) {
            return value.Value() * sqrt(viewPort.Width() * viewPort.Height());
        }
        return 0.0;
    }
    case DimensionUnit::PX:
        return value.Value();
    case DimensionUnit::VP:
        return vpToPx(value.Value());
    default:
        return vpToPx(value.Value());
    }
}

void SvgNode::Draw(OH_Drawing_Canvas *canvas) {
    // mask and filter create extra layers, need to record initial layer count
    const auto count = OH_Drawing_CanvasGetSaveCount(canvas);
    OH_Drawing_CanvasSave(canvas);
    if (!hrefClipPath_.empty()) {
        OnClipPath(canvas);
    }
    if (!attributes_.transform.empty()) {
        OnTransform(canvas);
    }
    if (!attributes_.maskId.empty()) {
        OnMask(canvas);
    }

    OnDraw(canvas);
    // on marker

    OnDrawTraversed(canvas);
    OH_Drawing_CanvasRestoreToCount(canvas, count);
}

void SvgNode::UpdateCommonProps(const ConcreteProps &props, const std::shared_ptr<SvgNode> &self) {
    attributes_.id = props->name;

    LOG(INFO) << "[UpdateCommonProps] opacity: " << props->opacity;
    if (hrefRender_) {
        attributes_.transform = props->matrix;
        attributes_.maskId = props->mask;
        attributes_.selfOpacity = props->opacity;
        attributes_.markerStart = props->markerStart;
        attributes_.markerMid = props->markerMid;
        attributes_.markerEnd = props->markerEnd;
        // clipPath
    }

    if (hrefFill_) {
        // auto href = attributes_.fillState.GetHref();
        // if (!href.empty()) {
        //   auto gradient = GetGradient(href);
        //   if (gradient) {
        //     attributes_.fillState.SetGradient(gradient.value());
        //   }
        // }
    }

    std::unordered_set<std::string> set;
    for (const auto& prop : props->propList) {
        set.insert(prop);
    }
    attributes_.fillState.SetColor(Color((uint32_t)*props->fill.payload), set.count("fill"));
    attributes_.fillState.SetOpacity(std::clamp(props->fillOpacity, 0.0, 1.0), set.count("fillOpacity"));
    attributes_.fillState.SetFillRule(std::to_string(props->fillRule), set.count("fillRule"));

    attributes_.strokeState.SetColor(Color((uint32_t)*props->stroke.payload), set.count("stroke"));
    attributes_.strokeState.SetLineWidth(vpToPx(StringUtils::StringToDouble(props->strokeWidth)), set.count("strokeWidth"));
    attributes_.strokeState.SetStrokeDashArray(StringUtils::stringVectorToDoubleVector(props->strokeDasharray),
                                               set.count("strokeDasharray"));
    attributes_.strokeState.SetStrokeDashOffset(vpToPx(props->strokeDashoffset), set.count("strokeDashoffset"));
    attributes_.strokeState.SetLineCap(SvgAttributesParser::GetLineCapStyle(std::to_string(props->strokeLinecap)),
                                       set.count("strokeLinecap"));
    attributes_.strokeState.SetLineJoin(SvgAttributesParser::GetLineJoinStyle(std::to_string(props->strokeLinejoin)),
                                        set.count("strokeLinejoin"));
    auto limit = vpToPx(props->strokeMiterlimit);
    if (GreatOrEqual(limit, 1.0)) {
        attributes_.strokeState.SetMiterLimit(limit, set.count("strokeMiterlimit"));
    }
    attributes_.strokeState.SetOpacity(std::clamp(props->strokeOpacity, 0.0, 1.0), set.count("strokeOpacity"));
}

void SvgNode::ContextTraversal(const std::shared_ptr<SvgNode> &self) {
    if (!attributes_.id.empty()) {
        context_->Push(attributes_.id, self);
    }
    for (const auto &child : children_) {
        child->SetContext(context_);
        child->ContextTraversal(child);
    }
}
} // namespace rnoh