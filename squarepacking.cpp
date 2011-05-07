#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

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

            // Step 2: No overlapping between squares, expressed with reification.
            BoolVarArgs rx(*this, N, 0, 1);
            BoolVarArgs ry(*this, N, 0, 1);

            for (int i = 0; i < x.size(); ++i) {
                for (int j = i + 1; j < y.size(); ++j) {
                    // Do something with rx/ry?
                }
            }

            // Step 3: Sum of square sizes for every row and column, expressed with reification.


            // Step 4: Additional propagators


            // Step 5: Branching heuristics
            //branch(*this, x, INT_VAR_MIN_MIN, INT_VAL_MIN);
            //branch(*this, y, INT_VAR_MIN_MIN, INT_VAL_MIN);
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
            os << "SquarePacking" << std::endl;
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
