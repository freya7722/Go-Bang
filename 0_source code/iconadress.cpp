#include "iconadress.h"

QString addressOfIcon(int icon)
{
    switch (icon) {
    case PIG:
        return ":/char/Image/bad-pig-icon_s.png";
    case OWL:
        return ":/char/Image/owl-icon_s.png";
    case RUBBER:
        return ":/char/Image/rubber-duck-icon_s.png";
    case SNOWMAN:
        return ":/char/Image/snowman-icon_s.png";
    case CAT_BANJO:
        return ":/char/Image/cat-banjo-icon-s.png";
    case CAT_CUPIE:
        return ":/char/Image/cat-cupid-love-icon-s.png";
    case CAT_LADIE:
        return ":/char/Image/cat-lady-icon-s.png";
    case CAT_FOOD:
        return ":/char/Image/cat-food-hearts-icon-s.png";
    case BANANA:
        return ":/char/Image/bananas-icon-s.png";

    default:
        return ":/char/Image/bad-pig-icon_s.png";
    }
}

QString addressOfIcon_Large(int icon)
{
    switch (icon) {
    case PIG:
        return ":/char/Image/bad-pig-icon.png";
    case OWL:
        return ":/char/Image/owl-icon.png";
    case RUBBER:
        return ":/char/Image/rubber-duck-icon.png";
    case SNOWMAN:
        return ":/char/Image/snowman-icon.png";
    case CAT_BANJO:
        return ":/char/Image/cat-banjo-icon.png";
    case CAT_CUPIE:
        return ":/char/Image/cat-cupid-love-icon.png";
    case CAT_LADIE:
        return ":/char/Image/cat-lady-icon.png";
    case CAT_FOOD:
        return ":/char/Image/cat-food-hearts-icon.png";
    case BANANA:
        return ":/char/Image/bananas-icon.png";
    default:
        return ":/char/Image/bad-pig-icon.png";

    }
}
