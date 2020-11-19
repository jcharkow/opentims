#include <cassert>
#include "opentims.cpp"
#include "converter.cpp"

int main(int argc, char** argv)
{
    assert(argc == 2);
    DefaultConverterFactory::setAsDefault<BrukerConverterFactory, const char*>("/home/lackistar/Projects/bruker/timsdata_scratchpad/tims2hdf5/libtimsdata.so");
    TimsDataHandle TDH(argv[1]);
    for(size_t ii = TDH.min_frame_id(); ii <= TDH.max_frame_id(); ii++)
    {
        TimsFrame& frame = TDH.get_frame(ii);

        frame.decompress();

    //    TDH.frame_desc(1).print();

        size_t s = frame.num_peaks;

        std::unique_ptr<uint32_t[]> frames = std::make_unique<uint32_t[]>(s);
        std::unique_ptr<uint32_t[]> scans  = std::make_unique<uint32_t[]>(s);
        std::unique_ptr<uint32_t[]> tofs   = std::make_unique<uint32_t[]>(s);
        std::unique_ptr<uint32_t[]> probs  = std::make_unique<uint32_t[]>(s);
        std::unique_ptr<double[]> masses  = std::make_unique<double[]>(s);

        frame.save_to_buffs(frames.get(), scans.get(), tofs.get(), probs.get(), masses.get(), nullptr, nullptr);

        for(size_t ii = 0; ii < s; ii++)
            std::cout << frames[ii] << "\t" << scans[ii] << "\t" << probs[ii] << "\t" << masses[ii] << std::endl;
//            std::cout << frames[ii] << "\t" << scans[ii] << "\t" << tofs[ii] << "\t" << probs[ii] << std::endl;
    }
}
