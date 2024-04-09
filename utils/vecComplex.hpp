#ifndef vecComplex
#define vecComplex

template <typename T>
class VecComplex {
    public:
        VecComplex(T real, T imag)
            : cached_real(real), cached_imag(imag),
            cached_real_sqared(real * real), cached_imag_squared(imag * imag) {}

        VecComplex mandelIterate(T c_real, T c_imag) {
            return VecComplex(cached_real_sqared - cached_imag_squared + c_real,
                              ((cached_real + cached_real) * cached_imag) + c_imag);
        }

        T norm() {
            return cached_real_sqared + cached_imag_squared;
        }

    private:
        T cached_real;
        T cached_imag;
        T cached_real_sqared;
        T cached_imag_squared;
};

#endif