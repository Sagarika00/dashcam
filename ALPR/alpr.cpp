#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "alpr.h"
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/imgproc/imgproc_c.h>

using namespace alpr;

class ALPRImageDetect {

private:
    cv::Mat image;
    std::string configFileName;
    std::string country;
    std::string region;
    alpr::Alpr* instance;
    alpr::AlprResults results;

    struct LicensePlate {
        std::string plateText;
        float confidence;
        bool match;
    };

public:

    const std::string extension = "jpg";
    static const float OVERALL_CONFIDENCE = 0.5f;

    ALPRImageDetect(cv::Mat i_image, std::string i_config, std::string i_country, std::string i_region)
    {
        image = i_image;
        configFileName = i_config;
        country = i_country;
        region = i_region;

        setInstance();
    }

    ALPRImageDetect(cv::Mat i_image, std::string i_config, std::string i_country)
    {
        image = i_image;
        configFileName = i_config;
        country = i_country;

        setInstance();
    }

    ~ALPRImageDetect() {}

    void setInstance()
    {
        alpr::Alpr* instance = new alpr::Alpr(country, configFileName);
    }

    static std::vector<alpr::AlprRegionOfInterest> constructRegionsOfInterest(std::vector<std::tuple<int, int, int, int>> regionsOfInterest)
    {
        int x, y, width, height;
        std::vector<AlprRegionOfInterest> regions(regionsOfInterest.size());
        for(int i = 0; i < regionsOfInterest.size(); i++) {
            std::tie(x, y, width, height) = regionsOfInterest[i];
            AlprRegionOfInterest regionOfInterest(x, y, width, height);
            regions[i] = regionOfInterest;
        }
        return regions;
    }

    static std::vector<LicensePlate> processPlates(AlprResults results, float overall_confidence = ALPRImageDetect::OVERALL_CONFIDENCE, bool matches_template = false)
    {
        std::vector<LicensePlate> licensePlates;
        for (int i = 0; i < results.plates.size(); i++)
        {
            AlprPlateResult plate = results.plates[i];
            for (int j = 0; j < plate.topNPlates.size(); j++)
            {
                AlprPlate candidate = plate.topNPlates[j];
                bool match = true ? (matches_template == false) : (candidate.matches_template == matches_template);
                if(candidate.overall_confidence > overall_confidence && match) {
                    licensePlates.push_back(LicensePlate{candidate.characters, candidate.overall_confidence, candidate.matches_template});
                }
            }
        }

        return licensePlates;
    }

    AlprResults detectAutonomousLicensePlate(int topN, std::vector<std::tuple<int, int, int, int>> regionsOfInterest)
    {
        instance->setTopN(topN);
        cv::Size size = image.size();
        void* buffer = malloc(size.width*size.height);
        std::vector<uchar>* buf = reinterpret_cast<std::vector<uchar>*>(buffer);
        cv::imencode(this->extension, image, *buf, {cv::IMWRITE_JPEG_OPTIMIZE});
        std::vector<AlprRegionOfInterest> RegionsOfInterest = ALPRImageDetect::constructRegionsOfInterest(regionsOfInterest);
        std::vector<char>* charbuf = reinterpret_cast<std::vector<char>*>(buf);
        AlprResults results = instance->recognize(*charbuf, RegionsOfInterest);
        return results;
    }

    std::vector<LicensePlate> detectLicensePlateFromRegion(int topN, std::string region, std::vector<std::tuple<int, int, int, int>> regionsOfInterest)
    {
        instance->setTopN(topN);
        instance->setDefaultRegion(region);
        cv::Size size = image.size();
        std::vector<AlprRegionOfInterest> RegionsOfInterest = ALPRImageDetect::constructRegionsOfInterest(regionsOfInterest);
        AlprResults results = instance->recognize(image.data, CV_8S, size.width, size.height, RegionsOfInterest);

        return ALPRImageDetect::processPlates(results);
    }

    std::vector<LicensePlate> detectLicensePlateMatches(int topN, std::vector<std::tuple<int, int, int, int>> regionsOfInterest)
    {
        AlprResults results = detectAutonomousLicensePlate(topN, regionsOfInterest);

        return ALPRImageDetect::processPlates(results, 0.5f, true);
    }

};
