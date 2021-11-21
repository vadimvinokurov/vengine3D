//
// Created by Boris on 10/18/2021.
//

#ifndef VENGINE_MATRIX33_HPP
#define VENGINE_MATRIX33_HPP

namespace VE {
    class Matrix33 {
    public:
        Matrix33() {
            setIdentity();
        };

        Matrix33(const float initValue[3][3]) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] = initValue[i][j];
                }
            }
        }

        float &operator()(size_t row, size_t col) {
            return a[row][col];
        }

        float operator()(size_t row, size_t col) const {
            return a[row][col];
        }

        void setZero() {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] = 0.0f;
                }
            }
        }

        void setIdentity() {
            a[0][0] = 1.0f;
            a[0][1] = 0.0f;
            a[0][2] = 0.0f;

            a[1][0] = 0.0f;
            a[1][1] = 1.0f;
            a[1][2] = 0.0f;

            a[2][0] = 0.0f;
            a[2][1] = 0.0f;
            a[2][2] = 1.0f;
        }

        Matrix33 operator+(float v) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] + v;
                }
            }
            return res;
        }

        Matrix33 operator-(float v) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] - v;
                }
            }
            return res;
        }

        Matrix33 operator*(float v) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] * v;
                }
            }
            return res;
        }

        Matrix33 operator/(float v) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] / v;
                }
            }
            return res;
        }

        Matrix33 operator+(const Matrix33 &b) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] + b(i, j);
                }
            }
            return res;
        }

        Matrix33 operator-(const Matrix33 &b) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    res(i, j) = a[i][j] - b(i, j);
                }
            }
            return res;
        }

        Matrix33 operator*(const Matrix33 &b) const {
            Matrix33 res;
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    for (size_t k = 0; k < 3; k++) {
                        res(i, j) += a[i][k] * b(k, j);
                    }
                }
            }
            return res;
        }

        Vector operator*(const Vector &b) const {
            Vector res;
            res.setX(a[0][0] * b.x() + a[0][1] * b.y() + a[0][2] * b.z());
            res.setY(a[1][0] * b.x() + a[1][1] * b.y() + a[1][2] * b.z());
            res.setZ(a[2][0] * b.x() + a[2][1] * b.y() + a[2][2] * b.z());
            return res;
        }

        Matrix33& operator+=(float v) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] += v;
                }
            }
            return *this;
        }

        Matrix33& operator-=(float v) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] -= v;
                }
            }
            return *this;
        }

        Matrix33& operator*=(float v) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] *= v;
                }
            }
            return *this;
        }

        Matrix33& operator/=(float v) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] /= v;
                }
            }
            return *this;
        }

        Matrix33& operator+=(const Matrix33 &b) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] += b(i, j);
                }
            }
            return *this;
        }

        Matrix33& operator-=(const Matrix33 &b) {
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    a[i][j] -= b(i, j);
                }
            }
            return *this;
        }

        Matrix33& operator*=(const Matrix33 &b) {
            *this = *this * b;
            return *this;
        }

        Matrix33 getTranspose() const {
            Matrix33 res;

            res(0, 0) = a[0][0];
            res(0, 1) = a[1][0];
            res(0, 2) = a[2][0];

            res(1, 0) = a[0][1];
            res(1, 1) = a[1][1];
            res(1, 2) = a[2][1];

            res(2, 0) = a[0][2];
            res(2, 1) = a[1][2];
            res(2, 2) = a[2][2];

            return res;
        }

        float getDeterminant() const {
            float p = a[0][0] * a[1][1] * a[2][2] + a[0][1] * a[1][2] * a[2][0] + a[0][2] * a[1][0] * a[2][1];
            float m = a[0][2] * a[1][1] * a[2][0] + a[0][0] * a[1][2] * a[2][1] + a[0][1] * a[1][0] * a[2][2];
            return p - m;
        }

        Matrix33 getInverse() const {
            Matrix33 res;
            float det = getDeterminant();
            if (det == 0.0f) return res;
            float invDet = 1.0f / det;

            res(0, 0) = a[1][1] * a[2][2] - a[2][1] * a[1][2];
            res(1, 0) = a[2][0] * a[1][2] - a[1][0] * a[2][2];
            res(2, 0) = a[1][0] * a[2][1] - a[2][0] * a[1][1];

            res(0, 1) = a[2][1] * a[0][2] - a[0][1] * a[2][2];
            res(1, 1) = a[0][0] * a[2][2] - a[2][0] * a[0][2];
            res(2, 1) = a[2][0] * a[0][1] - a[0][0] * a[2][1];

            res(0, 2) = a[0][1] * a[1][2] - a[1][1] * a[0][2];
            res(1, 2) = a[1][0] * a[0][2] - a[0][0] * a[1][2];
            res(2, 2) = a[0][0] * a[1][1] - a[1][0] * a[0][1];

            res *= invDet;

            return res;
        }

        void print() {
            std::cout << "\n";
            for (size_t i = 0; i < 3; i++) {
                for (size_t j = 0; j < 3; j++) {
                    std::cout << a[i][j] << "\t";
                }
                std::cout << "\n";
            }
        }

        virtual ~Matrix33() = default;

    private:
        float a[3][3]{};
    };
}


#endif //VENGINE_MATRIX33_HPP
