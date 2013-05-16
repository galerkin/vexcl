#include <iostream>
#include <vexcl/vexcl.hpp>

using namespace vex;

const size_t runs = 100;
const size_t repeats = 10;


void profile(Context &ctx, std::vector<size_t> size) {
    size_t n = std::accumulate(size.begin(), size.end(), 1, std::multiplies<size_t>());
    vector<cl_float2> a(ctx, n);
    vector<cl_float2> b(ctx, n);

    profiler prof(ctx);
    for(size_t i = 0 ; i < repeats ; i++) {
        prof.tic_cl("init");
        FFT<cl_float2> fft(ctx, size);
        fft.plan.profile = &prof;
        prof.toc("init");
        for(size_t j = 0 ; j < runs ; j++)
            b = fft(a);
    }
    std::cout << prof << std::endl;
}


int main() {
    try {
        Context ctx(Filter::Env && Filter::Count(1));
        std::cerr << ctx << std::endl;

        profile(ctx, {512, 512});
        profile(ctx, {521, 521});
    } catch(cl::Error &e) {
        std::cerr << e << std::endl;
    }

    return 0;
}