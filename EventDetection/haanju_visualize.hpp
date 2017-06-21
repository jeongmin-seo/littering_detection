/******************************************************************************
* Title        : haanju_visualize
* Author       : Haanju Yoo
* Initial Date : 2013.08.29 (ver. 0.9)
* Version Num. : 1.0 (since 2016.09.16)
* Description  : contains visualization helper functions
******************************************************************************
*               .__                           __.
*                \ '\~~---..---~~~~~~--.---~~| /
*                 '~-.   '                   .~         _____
*                     ~.                .--~~    .---~~~    /
*                      / .-.      .-.      |  <~~        __/
*                     |  |_|      |_|       \  \     .--'
*                    /-.      -       .-.    |  \_   \_
*                    \-'   -..-..-    '-'    |    \__  \_
*                     '.                     |     _/  _/
*                     ~-                .,-\   _/  _/
*                      /                 -~~~~\ /_  /_
*                     |               /   |    \  \_  \_
*                     |   /          /   /      | _/  _/
*                     |  |          |   /    .,-|/  _/
*                     )__/           \_/    -~~~| _/
*                       \                      /  \
*                        |           |        /_---'
*                        \    .______|      ./
*                        (   /        \    /
*                        '--'          /__/
*
******************************************************************************/

#ifndef __HAANJU_VISUALIZE_HPP__
#define __HAANJU_VISUALIZE_HPP__

#include "dku_types.hpp"

namespace hj
{

std::vector<cv::Scalar> GenerateColors(unsigned int numColor);
cv::Scalar hsv2rgb(double h, double s, double v);
cv::Scalar getColorByID(unsigned int nID, std::vector<cv::Scalar> *vecColors = NULL);
void DrawBoxWithID(cv::Mat &imageFrame, cv::Rect box, unsigned int nID, int lineStyle = 1, int fontSize = 0, std::vector<cv::Scalar> *vecColors = NULL);
void Draw3DBoxWithID(cv::Mat &imageFrame, std::vector<cv::Point> &pointArray, unsigned int nID, std::vector<cv::Scalar> *vecColors = NULL);
void DrawTriangleWithID(cv::Mat &imageFrame, cv::Point &point, unsigned int nID, std::vector<cv::Scalar> *vecColors = NULL);
void DrawLine(cv::Mat &imageFrame, std::vector<cv::Point> &pointArray, unsigned int nID, int lineThickness = 1, std::vector<cv::Scalar> *vecColors = NULL);
cv::Mat MakeMatTile(std::vector<cv::Mat> *imageArray, unsigned int numRows, unsigned int numCols);
cv::Point PointRescale(cv::Point _point, double _rescale, double _xmin = 0.0, double _ymin = 0.0);

}


#endif

//()()
//('')HAANJU.YOO


