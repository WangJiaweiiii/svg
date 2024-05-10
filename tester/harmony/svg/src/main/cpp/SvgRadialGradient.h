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

#ifndef FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_SVG_PARSE_SVG_LINEARGRADIENT_H
#define FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_SVG_PARSE_SVG_LINEARGRADIENT_H

#include "SVGGraphic.h"
#include "SvgSvg.h"
#include <native_drawing/drawing_point.h>

namespace rnoh {

class SvgRadialGradient : public SvgGraphic {
public:
    SvgRadialGradient();
    ~SvgRadialGradient() override = default;
    
    void OnDraw(OH_Drawing_Canvas *canvas) override;

    void SetAttrFx(const std::string& fx);
    void SetAttrFy(const std::string& fy);
    void SetAttrCx(const std::string& cx);
    void SetAttrCy(const std::string& cy);
    void SetAttrRx(const std::string& rx);
    void SetAttrRy(const std::string& ry);
    void SetAttrGradient(std::vector<double> gradient);
    void SetAttrGradientUnits(int gradientUnits);
    void SetAttrGradientTransforms(std::vector<double> gradientTransforms);

    const Gradient& GetGradient() const;

private:
    SvgGradientAttribute gradientAttr_;

};

} // namespace rnoh

#endif // FOUNDATION_ACE_FRAMEWORKS_CORE_COMPONENTS_NG_SVG_PARSE_SVG_LINEARGRADIENT_H