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
    const char* input_filename = "../two-channels-background-song-1.wav";
    const char* left_channel_filename = "left-channel-output.wav";
    const char* right_channel_filename = "right-channel-output.wav";
    // Open file as sequence of float`s, conversion is performed internally
    audio_reader_wav<float> reader(open_file_for_reading(input_filename));
    univector2d<float> audio = reader.read_channels();
println("Sample Rate  = ", reader.format().samplerate);
println("Channels     = ", reader.format().channels);
println("Length       = ", reader.format().length);
println("Duration (s) = ", reader.format().length / reader.format().samplerate);
println("Bit depth    = ", audio_sample_bit_depth(reader.format().type));
/*
    // Otvorite WAV fajl koristeći libsndfile
    SF_INFO sfinfo;
    SNDFILE* file = sf_open(input_filename, SFM_READ, &sfinfo);
    if (file == nullptr) {
        std::cerr << "Neuspesno otvaranje WAV fajla: " << sf_strerror(file) << std::endl;
        return 1;
    }

    // Ispisivanje informacija o fajlu
    std::cout << "Fajl je uspešno otvoren!" << std::endl;
    std::cout << "Broj kanala: " << sfinfo.channels << std::endl;
    std::cout << "Broj uzoraka: " << sfinfo.frames << std::endl;
    std::cout << "Sample rate: " << sfinfo.samplerate << std::endl;

    // Učitavanje podataka iz WAV fajla u vektor
    std::vector<float> audio_data(sfinfo.frames * sfinfo.channels);
    sf_readf_float(file, audio_data.data(), sfinfo.frames);

    // Zatvori WAV fajl nakon čitanja
    sf_close(file);

    // Kreiraj dva vektora za levi i desni kanal
    std::vector<float> left_channel(sfinfo.frames);
    std::vector<float> right_channel(sfinfo.frames);

    // Podeli audio podatke na dva kanala (pretpostavljamo stereo)
    for (size_t i = 0; i < sfinfo.frames; ++i) {
        left_channel[i] = audio_data[i * sfinfo.channels]; // Levi kanal je prvi uzorak svakog para
        right_channel[i] = audio_data[i * sfinfo.channels + 1]; // Desni kanal je drugi uzorak svakog para
    }

    // Spremanje levog kanala u novi WAV fajl
    SF_INFO left_info = sfinfo;
    left_info.channels = 1; // Samo jedan kanal za levi kanal
    SNDFILE* left_file = sf_open(left_channel_filename, SFM_WRITE, &left_info);
    if (left_file == nullptr) {
        std::cerr << "Neuspesno otvaranje levog izlaznog WAV fajla: " << sf_strerror(left_file) << std::endl;
        return 1;
    }
    sf_writef_float(left_file, left_channel.data(), sfinfo.frames);
    sf_close(left_file);

    // Spremanje desnog kanala u novi WAV fajl
    SF_INFO right_info = sfinfo;
    right_info.channels = 1; // Samo jedan kanal za desni kanal
    SNDFILE* right_file = sf_open(right_channel_filename, SFM_WRITE, &right_info);
    if (right_file == nullptr) {
        std::cerr << "Neuspesno otvaranje desnog izlaznog WAV fajla: " << sf_strerror(right_file) << std::endl;
        return 1;
    }
    sf_writef_float(right_file, right_channel.data(), sfinfo.frames);
    sf_close(right_file);

    std::cout << "Levi kanal sačuvan u: " << left_channel_filename << std::endl;
    std::cout << "Desni kanal sačuvan u: " << right_channel_filename << std::endl;
*/

    // Define an output univector with 1024 elements
    univector<fbase, 1024> output;
    // Create a 4th-order Butterworth bandpass filter with a passband from 0.005 to 0.9 (normalized frequency)
    zpk<fbase> filt = iir_bandpass(butterworth<fbase>(4), 0.005, 0.9);
    // Convert the filter to second-order sections (SOS).
    // This is an expensive operation, so keep 'iir_params' if it is reused later
    iir_params<fbase> bqs = to_sos(filt);

    // Apply the filter to a unit impulse signal to get the filter's impulse response
    //
    //output = iir(audio, bqs);




    return 0;
}
