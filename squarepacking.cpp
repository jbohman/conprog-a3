#include <gecode/driver.hh>
#include <gecode/int.hh>
#include <gecode/minimodel.hh>

using namespace Gecode;

class SquarePacking : public Script {
    protected:
        IntVar s;
        IntVarArray x;
        IntVarArray y;

    public:
        SquarePacking(const SizeOptions& opt) : x(*this, opt.size()), y(*this, opt.size()) { 
            //branch(*this, q, INT_VAR_RND, INT_VAL_MAX);
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
    opt.size(10);
    opt.parse(argc,argv);
    Script::run<SquarePacking, DFS, SizeOptions>(opt);
    return 0;
}
