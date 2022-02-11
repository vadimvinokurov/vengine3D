//
// Created by node on 13.08.2021.
//

#ifndef DEDITOR_COLOR_HPP
#define DEDITOR_COLOR_HPP

namespace VE {
    class Color {
    public:
        Color() : red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f) {}

        Color(float red_, float green_, float blue_, float alpha_ = 1.0f) : red(red_),
                                                                        green(green_),
                                                                        blue(blue_),
                                                                        alpha(alpha_) {}

        bool operator==(const Color &other) const {
            return (this->red == other.red) &&
                   (this->green == other.green) &&
                   (this->blue == other.blue) &&
                   (this->alpha == other.alpha);
        }

        bool operator!=(const Color &other) const {
            return !(*this == other);
        }

        const float* data() const {
            return v;
        }

        union {
            float v[4];
            struct{
                float red;
                float green;
                float blue;
                float alpha;
            };
        };
    };
}

#endif //DEDITOR_COLOR_HPP
