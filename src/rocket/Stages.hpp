#ifndef HEADER_rocket_Stages_hpp_ALREADY_INCLUDED
#define HEADER_rocket_Stages_hpp_ALREADY_INCLUDED

#include "rocket/Motor.hpp"
#include "gubg/debug.hpp"
#include <list>

namespace rocket { 

    enum class StageType
    {
        SideBurner, EndBurner,
    };

    class Stages
    {
        public:
            void add(StageType stage_type, unsigned int nr, double length, double in_diameter, bool insulated)
            {
                switch (stage_type)
                {
                    case StageType::SideBurner: add_stage_<SideBurner>(nr, length, in_diameter, insulated); break;
                    case StageType::EndBurner:  add_stage_<EndBurner>( nr, length, in_diameter, insulated); break;
                }
            }

            double out_diameter() const
            {
                double max_diam = 0.0;
                for (const auto &stage: stages_)
                    max_diam = std::max(max_diam, stage.out_diameter());
                return max_diam;
            }

            double mass() const
            {
                double m = 0.0;
                for (const auto &stage: stages_)
                    m += stage.mass();
                return m;
            }

            double thrust() const
            {
                if (stages_.empty())
                    //No more fuel
                    return 0.0;
                return stages_.front().thrust();
            }

            void process(double dt)
            {
                if (!stages_.empty())
                {
                    auto &stage = stages_.front();
                    if (stage.duration <= dt)
                        //This stage is burnt: drop it
                        stages_.pop_front();
                    else
                        //This stage is still burning, decrease its burn time
                        stage.duration -= dt;
                }
            }

        private:
            template <typename Motor>
                void add_stage_(unsigned int nr, double length, double in_diameter, bool insulated)
                {
                    stages_.emplace_back(nr);
                    auto &stage = stages_.back();

                    {
                        S("Stages");
                        Motor motor(length, in_diameter, insulated);
                        L(C(motor.name()));
                        L(C(motor.thrust()));
                        L(C(motor.full_mass()));
                        L(C(motor.burn_time()));
                        L(C(motor.propellant_mass()));
                        L(C(motor.out_diameter()));
                        stage.thr_duration = stage.duration = motor.burn_time();
                        stage.thr_one = motor.thrust();
                        stage.mass_propellant = motor.propellant_mass();
                        stage.mass_casing = motor.empty_mass();
                        stage.out_diam = motor.out_diameter();
                    }
                }

            struct Stage
            {
                double nr_thr = 0.0;

                double duration = 0.0;
                double thr_duration = 0.0;
                double thr_one = 0.0;
                double mass_propellant = 0.0;
                double mass_casing = 0.0;
                double out_diam = 0.0;
                double mass() const {return nr_thr*(mass_propellant*(duration/thr_duration)+mass_casing);}
                double thrust() const {return nr_thr*thr_one;}
                double out_diameter() const {return nr_thr*out_diam;}
                Stage(double nr_thr): nr_thr(nr_thr) {}
            };
            std::list<Stage> stages_;
    };

} 

#endif
