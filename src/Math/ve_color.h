//
// Created by node on 13.08.2021.
//

#ifndef DEDITOR_COLOR_HPP
#define DEDITOR_COLOR_HPP

namespace VE {
    class Color {
    public:
        Color() : red_(0.0f), green_(0.0f), blue_(0.0f), alpha_(0.0f) {}

        Color(float red, float green, float blue, float alpha = 1.0f) : red_(red),
                                                                        green_(green),
                                                                        blue_(blue),
                                                                        alpha_(alpha) {}

        Color(const Color &) = default;
        Color &operator=(const Color &) = default;

        bool operator==(const Color &other) const {
            return (this->red_ == other.red_) &&
                   (this->green_ == other.green_) &&
                   (this->blue_ == other.blue_) &&
                   (this->alpha_ == other.alpha_);
        }

        bool operator!=(const Color &other) const {
            return !(*this == other);
        }

        void setRed(float red) {
            red_ = red;
        }

        void setGreen(float green) {
            green_ = green;
        }

        void setBlue(float blue) {
            blue_ = blue;
        }

        void setAlpha(float alpha) {
            alpha_ = alpha;
        }

        float red() const {
            return red_;
        }

        float grean() const {
            return green_;
        }

        float blue() const {
            return blue_;
        }

        float alpha() const {
            return alpha_;
        }

    private:
        float red_ = 0.0f;
        float green_ = 0.0f;
        float blue_ = 0.0f;
        float alpha_ = 0.0f;
    };
}

#endif //DEDITOR_COLOR_HPP
