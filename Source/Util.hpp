template<typename Type>
struct RandomSeed {
    RandomSeed(Type Fixed,Type Variable):fixed(Fixed),variable(Variable){};
    RandomSeed(Type& Copy):fixed(Copy.fixed),variable(Copy.variable){};
    RandomSeed();
    Type fixed,variable;
};