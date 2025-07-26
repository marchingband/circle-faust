import("stdfaust.lib");
lfo = os.lf_triangle(0.1);
process = os.osc(440 + ((lfo + 1) * 500)) <: _,_;