#pragma once

#include <iomanip>
#include <gmpxx.h>

namespace randest {
    class spectral_test {
    public:
        static mpf_class run(mpz_class a, mpz_class c, mpz_class m, int T) {
            mpf_set_default_prec(1000);
            mpf_class *vi = new mpf_class[T+1];
            //S1 init
            int t = 2;
            mpz_class h = a,
                      hp = m,
                      p = 1,
                      pp = 0,
                      r = a,
                      s = a * a + 1;
            //S2 eucledean step
            mpz_class q, u, v;
            S2:
            q = hp / h;
            u = hp - q * h;
            v = pp - q * p;
            if (u * u + v * v < s) {
                s = u * u + v * v;
                hp = h;
                h = u;
                pp = p;
                p = v;
                goto S2;
            }
            //S3 compute v_2
            u = u - h;
            v = v - p;
            if (u * u + v * v < s) {
                s = u * u + v * v;
                hp = u;
                pp = v;
            }
            mpf_class helper = s;
            vi[2] = sqrt(helper);

            if (T == 2) return vi[T];

            //mpz_class U[T][T], V[T][T];
            //U[0][0] = -h; U[0][1] = p;
            //U[1][0] = -hp; U[0][1] = pp;
            mpz_class U[10][10] = {{0, 0},
                                {0, -h, p},
                                 {0, -hp, pp}};
            int sign = 1;
            if (pp > 0) sign = -1;
            mpz_class V[10][10] = {{0, 0},
                                {0, pp * sign, hp * sign},
                                 {0, -p * sign, -h * sign}};

            //S4
            int j, k;
            while (t < T) {
                t++;
                r = (a * r) % m;
                U[t][1] = -r;
                for (int n = 2; n < t; ++n) {
                    U[t][n] = 0;
                }
                U[t][t] = 1;

                for (int n = 1; n < t; ++n) {
                    U[n][t] = 0;
                    V[t][n] = 0;
                }
                V[t][t] = m;

                for (int n = 1; n < t; ++n) {
                    q = (V[n][1] * r) / m;
                    //mpz_class remainder = (V[n][1] * r) % m;
                    if (((V[n][1] * r) % m) * 2 > m) {
                        q = q + 1;
                    }
                    if (((V[n][1] * r) % m) * 2 < -m) {
                        q = q - 1;
                    }
                    V[n][t] = V[n][1] * r - q * m;
                    for (int e = 1; e <= t; ++e) {
                        U[t][e] = U[t][e] + q * U[n][e];
                    }
                }
                mpz_class cand = 0;
                for (int n = 1; n <= t; ++n) {
                    cand += U[t][n] * U[t][n];
                }
                if (cand < s) {
                    s = cand;
                }
                k = t;
                j = 1;
                //std::cout << "End of S4:\n";
                //                    for (int w = 1; w <= t; ++w) {
                //                        for (int b = 1; b <= t; ++b) {
                //                            std::cout << U[w][b] << ' ';
                //                        }
                //                        std::cout << '\n';
                //                    }
                //                    for (int w = 1; w <= t; ++w) {
                //                        for (int b = 1; b <= t; ++b) {
                //                            std::cout << V[w][b] << ' ';
                //                        }
                //                        std::cout << '\n';
                //                    }
                //std::cout << "Start of S5:\n";

                //S5
                S5:
                for (int i = 1; i <= t; ++i) {
                    if (i != j) {
                        mpz_class ijprod = 0;
                        mpz_class jjprod = 0;
                        for (int n = 1; n <= t; ++n) {
                            ijprod += V[i][n] * V[j][n];
                            jjprod += V[j][n] * V[j][n];
                        }
                        if (2 * abs(ijprod) > jjprod) {
                            q = ijprod / jjprod;
                            auto remainder = ijprod % jjprod;
                            if (remainder * 2 > jjprod) {
                                q = q + 1;
                            }
                            if (remainder * 2 < -jjprod) {
                                q = q - 1;
                            }
                            for (int n = 1; n <= t; ++n) {
                                V[i][n] = V[i][n] - q * V[j][n];
                                U[j][n] = U[j][n] + q * U[i][n];
                            }
                            cand = 0;
                            for (int n = 1; n <= t; ++n) {
                                cand += U[j][n] * U[j][n];
                            }
                            if (cand < s) {
                                s = cand;
                            }
                            k = j;
                            //std::cout << "---------------------\n";
                            //std::cout << k << " " << j << " " << i << " ----- " << q << '\n';
                            //        for (int w = 1; w <= t; ++w) {
                            //            for (int b = 1; b <= t; ++b) {
                            //                std::cout << U[w][b] << ' ';
                            //            }
                            //            std::cout << '\n';
                            //        }
                            //        for (int w = 1; w <= t; ++w) {
                            //            for (int b = 1; b <= t; ++b) {
                            //                std::cout << V[w][b] << ' ';
                            //            }
                            //            std::cout << '\n';
                            //        }
                            //std::cout << "---------------------\n";
                        }
                    }
                }

                //S6
                if (j == t) {
                    j = 1;
                } else {
                    j++;
                }
                if (j != k) goto S5;

                //S7
                mpz_class X[10], Y[10], Z[10];
                for (int n = 0; n < 10; ++n) X[n] = Y[n] = 0;
                k = t;
                for (int n = 1; n <= t; ++n) {
                    cand = 0;
                    for (int g = 1; g <= t; ++g) {
                        cand += V[j][g] * V[j][g];
                    }
                    Z[n] = sqrt(cand * s / (m * m));
                }
                
                //S8
                S8:
                if (X[k] == Z[k]) goto S10;
                else { 
                    X[k] = X[k] + 1;
                    for (int n = 1; n <= t; ++n) {
                        Y[n] = Y[n] + U[k][n];
                    }
                }
                //S9
                S9:
                k++;
                if (k < t) {
                    X[k] = -Z[k];
                    for (int n = 1; n <= t; ++n) {
                        Y[n] = Y[n] - 2 * Z[k] * U[k][n];
                    }
                    goto S9;
                } else {
                    cand = 0;
                    for (int g = 1; g <= t; ++g) {
                        cand += Y[g] * Y[g];
                    }
                    if (cand < s)
                        s = cand;
                }
                S10:
                k = k - 1;
                if (k >= 1) {
                    goto S8;
                }
                else {
                    mpf_class helper = s;
                    vi[t] = sqrt(helper);
                }
            }

            return vi[T];
        }
    };
}
