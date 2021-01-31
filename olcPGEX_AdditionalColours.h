#pragma once

#include "olcPixelGameEngine.h"

namespace olc
{
    namespace Colours
    {
        //ALL THE COLORS OF THE WIND!
        static const Pixel
            OFF_BLACK(24, 24, 26),
            OFF_WHITE(239, 248, 254),

            ORANGE(255, 140, 0),
            DARK_ORANGE(255, 127, 0),
            VERY_DARK_ORANGE(255, 114, 0),

            APPLE_GREEN(206, 255, 89),
            DARK_APPLE_GREEN(174, 222, 0),
            VERY_DARK_APPLE_GREEN(84, 107, 0),

            COBALT_BLUE(0, 160, 255),
            DARK_COBALT_BLUE(0, 97, 176),
            VERY_DARK_COBALT_BLUE(28, 56, 92),

            PURPLE(145, 61, 255),
            DARK_PURPLE(96, 0, 222),
            VERY_DARK_PURPLE(49, 0, 102),

            PINK(255, 121, 186),
            DARK_PINK(235, 0, 109),
            VERY_DARK_PINK(107, 0, 43),

            BROWN(199, 89, 27),
            DARK_BROWN(117, 37, 0),
            VERY_DARK_BROWN(51, 13, 0),

            CORNFLOWER_BLUE(100, 149, 237),
            DARK_CORNFLOWER_BLUE(80, 129, 217),
            VERY_DARK_CORNFLOWER_BLUE(60, 109, 197),

            BIDOOF(242, 225, 179),
            DARK_BIDOOF(156, 117, 58),
            VERY_DARK_BIDOOF(81, 68, 62),

            PALE_RED(255, 179, 183),
            PALE_ORANGE(255, 229, 187),
            PALE_YELLOW(255, 253, 181),
            PALE_GREEN(202, 255, 192),
            PALE_CYAN(192, 255, 240),
            PALE_COBALT(194, 220, 255),
            PALE_PURPLE(208, 186, 255),
            PALE_PINK(255, 178, 216),

            PALE_DARK_RED(130, 66, 61),
            PALE_DARK_ORANGE(133, 94, 61),
            PALE_DARK_YELLOW(133, 117, 70),
            PALE_DARK_GREEN(110, 133, 67),
            PALE_DARK_CYAN(78, 133, 112),
            PALE_DARK_COBALT(75, 114, 133),
            PALE_DARK_PURPLE(83, 69, 133),
            PALE_DARK_PINK(130, 66, 133);

        // Taken from https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
        // Creates a new Pixel instance from hsv space (hue, saturation and value).
        // Hue ranges from 0 to 360, saturation and value from 0 to 1.
        static olc::Pixel FromHsv(float hue, float saturation, float value, const float alpha = 1)
        {
            hue = std::clamp(hue, 0.0f, 360.0f);
            saturation = std::clamp(saturation, 0.0f, 1.0f);
            value = std::clamp(value, 0.0f, 1.0f);
            
            const float chroma = value * saturation;
            const float x = chroma * (1 - std::fabs(fmodf((hue / 60), 2) - 1));
            const float m = value - chroma;

            float rawRed, rawGreen, rawBlue;

            if (saturation == 0) {
                rawRed = rawGreen = rawBlue = value;
            }           
            else if (hue < 60) {
                rawRed = chroma;
                rawGreen = x;
                rawBlue = 0;
            }
            else if (60 <= hue && hue < 120) {
                rawRed = x;
                rawGreen = chroma;
                rawBlue = 0;
            }
            else if (120 <= hue && hue < 180) {
                rawRed = 0;
                rawGreen = chroma;
                rawBlue = x;
            }
            else if (180 <= hue && hue < 240) {
                rawRed = 0;
                rawGreen = x;
                rawBlue = chroma;
            }
            else if (240 <= hue && hue < 300) {
                rawRed = x;
                rawGreen = 0;
                rawBlue = chroma;
            }
            else {
                rawRed = chroma;
                rawGreen = 0;
                rawBlue = x;
            }

            return olc::PixelF(rawRed + m , rawGreen + m , rawBlue + m , alpha);
        }
    }
}