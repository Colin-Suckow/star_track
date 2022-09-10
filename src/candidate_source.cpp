#include "candidate_source.hpp"

#include <string>
#include <math.h>

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/core/types.hpp>

CandidateSource::CandidateSource(std::string const& img_path, Camera camera) : camera(camera)
{
    src_img = cv::imread(img_path, cv::IMREAD_GRAYSCALE);
    process_img();
    detect_stars();
}

CandidateSource::CandidateSource(std::vector<CandidateStar> candidates, Camera camera) : candidates(candidates), camera(camera) {}

void CandidateSource::process_img()
{
    cv::threshold(src_img, src_img, 60, 255, cv::THRESH_BINARY);
    cv::bitwise_not(src_img, src_img);
}

void CandidateSource::detect_stars()
{
    auto params = cv::SimpleBlobDetector::Params();
    auto detector = cv::SimpleBlobDetector::create(params);
    std::vector<cv::KeyPoint> key_points;
    detector->detect(src_img, key_points);
    for (auto& key_point : key_points)
    {
        candidates.push_back(CandidateStar(key_point.pt.x, key_point.pt.y));
    }
}

unsigned int CandidateSource::size()
{
    return candidates.size();
}

double CandidateSource::pixel_distance_between(CandidateStar& s1, CandidateStar& s2)
{
    return sqrt(abs(pow(s1.x() - s2.x(), 2.) + pow(s1.y() - s2.y(), 2.)));
}

double CandidateSource::angular_distance_between(CandidateStar& s1, CandidateStar& s2)
{
    auto px_dist = pixel_distance_between(s1, s2);
    return atan((px_dist * camera.avg_pixel_size) / camera.focal_len_mm);
}

std::vector<CandidateStar>& CandidateSource::get_candidates()
{
    return candidates;
}