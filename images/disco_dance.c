/* GIMP RGB C-Source image dump (disco_dance.c) */

static const struct
{
  unsigned int width;
  unsigned int height;
  unsigned int bytes_per_pixel; /* 2:RGB16, 3:RGB, 4:RGBA */
  unsigned char pixel_data[25 * 25 * 3 + 1];
} disco_dance = {
    25,
    25,
    3,
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\000\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\274\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\377\000"
    "\000\377\000\000\315\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\336\000\000"
    "\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\377\000\000\377\000\000\000\000\000\377\000\000"
    "\377\000\000\000\000\000\315\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000"
    "\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\000\000\000\377\000\000\377\000\000\000\000\000\315\000\000\377\000\000\000\000\000\377\000\000\377\000\000\000"
    "\000\000\377\000\000\377\000\000\000\000\000\315\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\000\000\000\000\000\000\000\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\315\000\000\377\000\000\315\000\000\000\000\000\377\000\000\000\000\000\000\000\000\377\000"
    "\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\377\000\000\000\000"
    "\000\377\000\000\000\000\000\353\000\000\377\000\000\000\000\000\377\000\000\311\000\000\000\000\000\377\000\000\377"
    "\000\000\000\000\000\324\000\000\000\000\000\315\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\315\000\000\000\000\000\341\000\000\000\000\000\377\000\000\000\000\000\014\000\000\377\000\000\000\000"
    "\000\000\000\000\000\000\000\000\000\000\377\000\000\377\000\000\014\000\000\000\000\000\000\000\000\377\000\000\000\000\000\000\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\315\000\000\000\000\000\377\000\000\000\000\000\000"
    "\000\000\315\000\000\000\000\000\000\000\000\315\000\000\377\000\000\377\000\000\377\000\000\315\000\000\000\000\000\000"
    "\000\000\000\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\000\000\000\000\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    "\000\000\000\000\000\000\000\000\000\000\315\000\000\377\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\315\000\000\315\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\315\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\315\000\000\000\000\000\315\000\000\377\000\000\377\000\000\377\000\000\377\000"
    "\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000"
    "\000\377\000\000\377\000\000\377\000\000\000\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\000\000\000\000"
    "\000\000\000\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\000\000\000\377\000\000\377\000\000\377\000\000\000\000\000\377\000\000\377\000\000\000\000\000\377\000\000\377"
    "\000\000\377\000\000\000\000\000\000\000\000\315\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000"
    "\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\315\000\000\377\000\000\377\000\000\377"
    "\000\000\000\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000\000\315\000\000\000"
    "\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000"
    "\000\000\000\000\000\000\000\315\000\000\315\000\000\315\000\000\372\000\000\000\000\000\372\000\000\377\000\000\377"
    "\000\000\377\000\000\000\000\000\000\000\000\000\000\000\377\000\000\377\000\000\000\000\000\000\000\000\377\000\000\377\000"
    "\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
    "\000\000\000\000\000\000\324\000\000\377\000\000\377\000\000\377\000\000\377\000\000\000\000\000\000\000\000\000\000\000\000"
    "\000\000\000\000\000\000\000\000\000\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\315\000\000\315\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377\000\000\377"
    "\000\000\377\000\000",
};
