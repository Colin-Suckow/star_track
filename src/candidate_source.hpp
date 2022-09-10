#ifndef CANDIDATE_SOURCE_H
#define CANDIDATE_SOURCE_H

#include <opencv2/imgcodecs.hpp>
#include <string>

struct Camera
{
    double focal_len_mm, sensor_width_mm, sensor_height_mm, avg_pixel_size;
    unsigned int res_x, res_y;

    Camera(double focal_len_mm, double sensor_width_mm, double sensor_height_mm, unsigned int res_x, unsigned int res_y)
    : focal_len_mm(focal_len_mm), sensor_width_mm(sensor_width_mm), sensor_height_mm(sensor_height_mm), res_x(res_x), res_y(res_y)
    {
        // Average the x and y pixel sizes. This is good enough for my camera since the pixels are almost square
        avg_pixel_size = ((sensor_width_mm / res_x) + (sensor_height_mm / res_y)) / 2.0;
    }
};

class CandidateStar
{
    double px_x, px_y;
public:
    CandidateStar(double px_x, double px_y) : px_x(px_x), px_y(px_y) {}
    constexpr double x() { return px_x; }
    constexpr double y() { return px_y; }
};

class CandidateSource
{
    cv::Mat src_img;
    std::vector<CandidateStar> candidates;
    Camera camera;
public:
    CandidateSource(std::string const& img_path, Camera camera);
    CandidateSource(std::vector<CandidateStar> candidates, Camera camera);
    unsigned int size();
    double pixel_distance_between(CandidateStar& s1, CandidateStar& s2);
    double angular_distance_between(CandidateStar& s1, CandidateStar& s2);
    std::vector<CandidateStar>& get_candidates();
private:
    void process_img();
    void detect_stars();
};

#endif /* CANDIDATE_SOURCE_H */
