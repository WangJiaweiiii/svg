/*
 * Copyright (c) 2022-2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "SvgRadialGradient.h"
#include <native_drawing/drawing_shader_effect.h>

namespace rnoh {

SvgRadialGradient::SvgRadialGradient() {
    gradientAttr_.gradient.SetType(GradientType::RADIAL);
}

void SvgRadialGradient::OnDraw(OH_Drawing_Canvas *canvas) 
{
    LOG(INFO) << "javivi SvgRadialGradient::OnDraw";
    LOG(INFO) << "javivi SvgRadialGradient fx:" << gradientAttr_.gradient.GetRadialGradient().fRadialCenterX->Value()
        << " fy:" << gradientAttr_.gradient.GetRadialGradient().fRadialCenterY->Value()
        << " cx:" << gradientAttr_.gradient.GetRadialGradient().radialCenterX->Value()
        << " cy:" << gradientAttr_.gradient.GetRadialGradient().radialCenterY->Value()
        << " rx:" << gradientAttr_.gradient.GetRadialGradient().radialHorizontalSize->Value()
        << " ry:" << gradientAttr_.gradient.GetRadialGradient().radialVerticalSize->Value();
    
}

void SvgRadialGradient::SetAttrFx(const std::string& fx) 
{
    gradientAttr_.gradient.GetRadialGradient().fRadialCenterX = SvgAttributesParser::ParseDimension(fx);
}

void SvgRadialGradient::SetAttrFy(const std::string& fy) 
{
    gradientAttr_.gradient.GetRadialGradient().fRadialCenterY = SvgAttributesParser::ParseDimension(fy);
}

void SvgRadialGradient::SetAttrCx(const std::string& cx) 
{
    gradientAttr_.gradient.GetRadialGradient().radialCenterX = SvgAttributesParser::ParseDimension(cx);
}

void SvgRadialGradient::SetAttrCy(const std::string& cy) 
{
    gradientAttr_.gradient.GetRadialGradient().radialCenterY = SvgAttributesParser::ParseDimension(cy);
}

void SvgRadialGradient::SetAttrRx(const std::string& rx) 
{
    gradientAttr_.gradient.GetRadialGradient().radialHorizontalSize = SvgAttributesParser::ParseDimension(rx);
}

void SvgRadialGradient::SetAttrRy(const std::string& ry) 
{
    gradientAttr_.gradient.GetRadialGradient().radialVerticalSize = SvgAttributesParser::ParseDimension(ry);
}

void SvgRadialGradient::SetAttrGradient(std::vector<double> gradient) 
{
    auto stopCount = gradient.size() / 2;
    for (auto i = 0; i < stopCount; i++) {
        auto stopIndex = i * 2;
        GradientColor gradientColor;
        gradientColor.SetDimension(Dimension(gradient[stopIndex]));
        gradientColor.SetColor(Color((int32_t)gradient[stopIndex + 1]));
        gradientAttr_.gradient.AddColor(gradientColor);
    }
}

void SvgRadialGradient::SetAttrGradientUnits(int gradientUnits) 
{
    // gradientAttr_.gradient.setGradientUnits(gradientTransforms);
}

void SvgRadialGradient::SetAttrGradientTransforms(std::vector<double> gradientTransforms) 
{
    // gradientAttr_.gradient.SetGradientTransform(gradientTransforms);
}

const Gradient& SvgRadialGradient::GetGradient() const
{
    return gradientAttr_.gradient;
}


}