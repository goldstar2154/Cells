#pragma once

struct pixel {
    union
    {
        struct
        {
            // BGR instead RGB... such microsoft (doge)
            unsigned char b, g, r, a;
        };
        int val;
    };
    pixel()
    {
        val = 0;
    }
};