#include <dlib/svm_threaded.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_processing.h>
#include <dlib/data_io.h>
#include <dlib/cmd_line_parser.h>

using namespace std;
using namespace dlib;

static const char *const help_key = "h";
static const char *const upsample_key = "u";
static const char *const wait_key = "wait";

struct TrafficSign {
    string name;
    string svm_path;
    rgb_pixel color;

    TrafficSign(string name, string svm_path, rgb_pixel color) : name(name), svm_path(svm_path), color(color) {};
};

int main(int argc, char **argv) {
    try {
        command_line_parser parser;

        parser.add_option(help_key, "Display this help message.");
        parser.add_option(upsample_key,
                          "Upsample each input image <arg> times. Each upsampling quadruples the number of pixels in the image (default: 0).",
                          1);
        parser.add_option(wait_key, "Wait user input to show next image.");

        parser.parse(argc, argv);
        parser.check_option_arg_range(upsample_key, 0, 8);

        const char *one_time_opts[] = {help_key, upsample_key, wait_key};
        parser.check_one_time_options(one_time_opts);

        // Display help message
        if (parser.option(help_key)) {
            cout << "Usage: " << argv[0] << " [options] <list of images>" << endl;
            parser.print_options();

            return EXIT_SUCCESS;
        }

        // Display help direction
        if (parser.number_of_arguments() == 0) {
            cout << "You must give a list of input files." << endl;
            cout << "\nTry the -" << help_key << " option for more information." << endl;

            return EXIT_FAILURE;
        }

        const long upsample_amount = get_option(parser, upsample_key, 0);

        dlib::array<array2d<unsigned char>> images;

        images.resize(parser.number_of_arguments());

        for (unsigned long i = 0; i < images.size(); ++i) {
            load_image(images[i], parser[i]);
        }

        for (unsigned long i = 0; i < upsample_amount; ++i) {
            for (unsigned long j = 0; j < images.size(); ++j) {
                pyramid_up(images[j]);
            }
        }

        typedef scan_fhog_pyramid<pyramid_down<6>> image_scanner_type;

        // Load SVM detectors
        std::vector<TrafficSign> signs;
        signs.push_back(TrafficSign("PARE", "svm_detectors/pare_detector.svm",
                                    rgb_pixel(255, 0, 0)));
        signs.push_back(TrafficSign("PROIBIDO ESTACIONAR", "svm_detectors/proibido_estacionar_detector.svm",
                                    rgb_pixel(255, 122, 0)));

        std::vector<object_detector<image_scanner_type> > detectors;

        for (int i = 0; i < signs.size(); i++) {
            object_detector<image_scanner_type> detector;
            deserialize(signs[i].svm_path) >> detector;
            detectors.push_back(detector);
        }

        image_window win;
        std::vector<rect_detection> rects;
        for (unsigned long i = 0; i < images.size(); ++i) {
            evaluate_detectors(detectors, images[i], rects);

            // Put the image and detections into the window.
            win.clear_overlay();
            win.set_image(images[i]);

            for (unsigned long j = 0; j < rects.size(); ++j) {
                win.add_overlay(rects[j].rect, signs[rects[j].weight_index].color,
                                signs[rects[j].weight_index].name);
            }

            if (parser.option(wait_key)) {
                cout << "Press any key to continue...";
                cin.get();
            }
        }
    }
    catch (exception &e) {
        cout << "\nexception thrown!" << endl;
        cout << e.what() << endl;
    }
}