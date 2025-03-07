#include <sndfile.h>
#include <iostream>
#include <kfr/base.hpp>
#include <kfr/dsp.hpp>
#include <kfr/io.hpp>
#include <vector>
#include <cmath> // za generisanje sinusoida

using namespace kfr;
int main() {
    // Putanja do WAV fajla
    const char* input_filename1 = "../two-channels-background-song-1.wav";
    const char* left_channel_filename = "left-channel-output.wav";
    const char* right_channel_filename = "right-channel-output.wav";
    const char* input_filename2 = "filtered-output.wav";

    
    // Otvorite WAV fajl kao sekvencu float vrednosti
    auto reader = kfr::audio_reader_wav<float>(open_file_for_reading(input_filename1));
    const auto& format = reader.format();
    
    println("Sample Rate  = ", format.samplerate);
    println("Channels     = ", format.channels);
    println("Length       = ", format.length);
    println("Duration (s) = ", format.length / format.samplerate);
    println("Bit depth    = ", audio_sample_bit_depth(format.type));
    
    // Učitajte audio podatke za oba kanala
    univector2d<float> audio1 = reader.read_channels();

    // Uzmite levi kanal
    univector<float> left_channel = audio1[0];  // Levi kanal je prvi u univector2d

    // Uzmite desni kanal
    univector<float> right_channel = audio1[1];  // Desni kanal je drugi u univector2d

    // Proverite da li je broj uzoraka za oba kanala jednak originalnom broju uzoraka
    std::cout << "Levi kanal ima " << left_channel.size() << " uzoraka." << std::endl;
    std::cout << "Desni kanal ima " << right_channel.size() << " uzoraka." << std::endl;

    // Proverite da li je broj uzoraka za oba kanala isti kao originalni broj uzoraka
    if (left_channel.size() != format.length || right_channel.size() != format.length) {
        std::cerr << "Greška: broj uzoraka za kanale se ne poklapa sa originalnim brojem uzoraka." << std::endl;
        return -1;
    }

        // Postavite novi format sa jednim kanalom
    audio_format_and_length mono_format = format;
    mono_format.channels = 1;
    mono_format.length = format.length; // Ostaviti dužinu istu kao originalni fajl

    // Otvorite fajlove za pisanje
    auto left_writer = std::make_shared<audio_writer_wav<float>>(open_file_for_writing(left_channel_filename), mono_format);
    auto right_writer = std::make_shared<audio_writer_wav<float>>(open_file_for_writing(right_channel_filename), mono_format);
    float absmax = 0;
    for (size_t i = 0; i < left_channel.size(); ++i)
    {
        absmax = std::max(absmax, std::abs(left_channel[i]));
    }
    for (size_t i = 0; i < left_channel.size(); ++i)
    {
        left_channel[i] = left_channel[i] / absmax;
    }
    // Upisivanje podataka u izlazne fajlove
    left_writer->write(left_channel.data(), left_channel.size());
    right_writer->write(right_channel.data(), right_channel.size());
    

    // Zatvaranje fajlova
    left_writer->close();
    right_writer->close();

    // Define an output univector with 1024 elements
    univector<float> output;
    
    {
        zpk<fbase> filt = iir_bandpass(butterworth<fbase>(4), 30, 130);



        // Convert the filter to second-order sections (SOS).
        // This is an expensive operation, so keep 'iir_params' if it is reused later
        iir_params<fbase> bqs = to_sos(filt);

        // Apply the filter to a unit impulse signal to get the filter's impulse response
        output = iir(left_channel, bqs);
    }

    auto filtered_writer= std::make_shared<audio_writer_wav<float>>(open_file_for_writing(input_filename2), mono_format);
    filtered_writer->write(output.data(),output.size());
    filtered_writer->close();
    absmax = 0;
    for (size_t i = 0; i < output.size(); ++i)
    {
        absmax = std::max(absmax, std::abs(output[i]));
    }
    for (size_t i = 0; i < output.size(); ++i)
    {
        output[i] = output[i] / absmax;
    }
    
    
    
    for(int i=20000;i<33000;i++){
        std::cout<<output.data()[i]<<"-----"<<left_channel.data()[i]<<std::endl;
    }

    
    return 0;
}
