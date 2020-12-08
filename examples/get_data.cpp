#include <string>
#include "../opentims++/opentims_all.cpp"

const std::string data_path = "/path/to/your/data.d";

const bool use_bruker_code = false;
const std::string bruker_binary_lib_path = "";

int main()
{
    // If we're using Bruker's conversion functions, they must be set up before opening the TimsDataHandle
    if(use_bruker_code)
    {
        DefaultTof2MzConverterFactory::setAsDefault<BrukerTof2MzConverterFactory, const char*>(bruker_binary_lib_path.c_str());
        DefaultScan2InvIonMobilityConverterFactory::setAsDefault<BrukerScan2InvIonMobilityConverterFactory, const char*>(bruker_binary_lib_path.c_str());
    }

    // Open the dataset
    TimsDataHandle TDH(data_path);


    // Allocate buffers for data: instead of reallocating for every frame, we just allocate a buffer that will fit all
    // and reuse it.
    const size_t buffer_size_needed = TDH.max_peaks_in_frame(); 

    std::unique_ptr<uint32_t[]> frame_ids = std::make_unique<uint32_t[]>(buffer_size_needed);
    std::unique_ptr<uint32_t[]> scan_ids = std::make_unique<uint32_t[]>(buffer_size_needed);
    std::unique_ptr<uint32_t[]> tofs = std::make_unique<uint32_t[]>(buffer_size_needed);
    std::unique_ptr<uint32_t[]> intensities = std::make_unique<uint32_t[]>(buffer_size_needed);

    std::unique_ptr<double[]> mzs = use_bruker_code ? std::make_unique<double[]>(buffer_size_needed) : nullptr;
    std::unique_ptr<double[]> inv_ion_mobilities = use_bruker_code ? std::make_unique<double[]>(buffer_size_needed) : nullptr;

    std::unique_ptr<double[]> retention_times = std::make_unique<double[]>(buffer_size_needed);

    // Print table header
    std::cout << "Frame ID" << "\t" << "Scan ID" << "\t" << "Time-of-flight" << "\t" << "Intensity" << "\t";
    if(use_bruker_code)
        std::cout << "MZ" << "\t" << "Inverse of ion mobility" << "\t";
    std::cout << "Retention time" << std::endl;


    // Obtain frames: as a map frame_id -> frame object
    std::unordered_map<uint32_t, TimsFrame>& frame_desc = TDH.get_frame_descs();
    for(auto& [_, frame] : frame_desc)
    {
        frame.save_to_buffs(frame_ids.get(), scan_ids.get(), tofs.get(), intensities.get(), mzs.get(), inv_ion_mobilities.get(), retention_times.get());
        for(size_t peak_id = 0; peak_id < frame.num_peaks; peak_id++)
        {
            std::cout << frame_ids[peak_id] << "\t" << scan_ids[peak_id] << "\t" << tofs[peak_id] << "\t" << intensities[peak_id] << "\t";
            if(use_bruker_code)
                std::cout << mzs[peak_id] << "\t" << inv_ion_mobilities[peak_id] << "\t";
            std::cout << retention_times[peak_id] << std::endl;
        }
    }

}
