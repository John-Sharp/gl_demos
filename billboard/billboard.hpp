#ifndef BILLBOARD_H
#define BILLBOARD_H

class Billboard {
    public:
        Billboard(
            const BoalerVSLModelUnitLink &mother,
            double mother_radius
                );

        BoalerVSLModelUnitLink &mother;
        double mother_radius;

        BoalerModelUnit model_unit;

        void update_pos();

#endif
