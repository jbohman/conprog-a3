#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>
#include <cstdio>

using namespace Gecode;

int N;

class SquarePacking : public Script {
    protected:
        IntVar s;
        IntVarArray x;
        IntVarArray y;
        static int size(int square_number) {
            return N - square_number;
        }

    public:
        SquarePacking(const SizeOptions& opt) : x(*this, opt.size()), y(*this, opt.size()) {
            // Step 1: Variables s, x, y and static size method.
            int min = 0;
            int max = 0;
            for (int i = 0; i < opt.size(); ++i) {
                min += i*i;
                max += i;
            }
            s = IntVar(*this, sqrt(min), max);

            for (int i = 0; i < opt.size(); ++i) {
                x[i] = IntVar(*this, 0, max);
                y[i] = IntVar(*this, 0, max);
            }

            for (int i = 0; i < N; ++i) {
                rel(*this, (x[i] + size(i) )<= s);
                rel(*this, (y[i] + size(i) )<= s);
            }

            // Step 2: No overlapping between squares, expressed with reification.
            for (int i = 0; i < N; i++) {
               for (int j = i+1; j < N; j++) {
                   rel(*this, (x[i] + size(i) <= x[j]) || (x[j] + size(j) <= x[i]) ||
                       (y[i] + size(i) <= y[j]) || (y[j] + size(j) <= y[i]));  
                }
            }

            // Step 3: Sum of square sizes for every row and column less than s, expressed with reification.

            IntArgs sizes(N);
            for (int i = 0; i < N; ++i) {
                sizes[i] = size(i);
            }

            for (int col = 0; col < s.max(); ++col) {
                BoolVarArgs bx(*this, N, 0, 1);
                for (int i = 0; i < N; ++i) {
                    dom(*this, y[i], col - size(i) + 1, col, bx[i]);
                }
                linear(*this, sizes, bx, IRT_LQ, s);
            }

            for (int col = 0; col < s.max(); ++col) {
                BoolVarArgs bx(*this, N, 0, 1);
                for (int i = 0; i < N; ++i) {
                    dom(*this, x[i], col - size(i) + 1, col, bx[i]);
                }
                linear(*this, sizes, bx, IRT_LQ, s);
            }

            // Step 4: Additional propagators
            
            // Step 4.1: Problem decomposition

            // Step 4.2: Symmetry removal

            // Step 4.3: Empty strip dominance

            // Step 4.4: Ignoring size 1 squares


            // Step 5: Branching heuristics
            branch(*this, s, INT_VAL_MIN);
            branch(*this, x, INT_VAR_MIN_MIN, INT_VAL_MIN);
            branch(*this, y, INT_VAR_MIN_MIN, INT_VAL_MIN);
        }

        SquarePacking(bool share, SquarePacking& sp) : Script(share, sp) {
            x.update(*this, share, sp.x);
            y.update(*this, share, sp.y);
            s.update(*this, share, sp.s);
        }

        virtual Space* copy(bool share) {
            return new SquarePacking(share, *this);
        }

        virtual void print(std::ostream& os) const {
            printf("s = %d\n", s.val());
            for (int i = 0; i < N; ++i) {
                os << size(i) << + "\t(" << x[i] << ", " << y[i] << ")" << std::endl;
            }

            char output[s.val()+1][s.val()+1];
            memset(output, 0, sizeof(output));

            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < size(i); ++j) {
                    for (int k = 0; k < size(i); ++k) {
                        output[x[i].val()+j][y[i].val()+k] = size(i);
                    }
                }
            }

            for (int i = 0; i < s.val()+1; ++i) {
                for (int j = 0; j < s.val()+1; ++j) {
                    if (output[i][j] == 0)
                        printf("  ");
                    else
                        printf("%2d", output[i][j]);
                }
                printf("\n");
            }
        }
};

int main(int argc, char* argv[]) {
    SizeOptions opt("SquarePacking");
    opt.size(6);
    opt.parse(argc,argv);
    N = opt.size();
    Script::run<SquarePacking, DFS, SizeOptions>(opt);
    return 0;
}
