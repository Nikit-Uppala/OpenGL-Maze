#ifndef LINE_H
#define LINE_H
#endif

class Line
{
    public:
        Line() {}
        Line(float* vertices);
        void bindData(unsigned int& VAO, unsigned int& VBO);
    private:
        float* vertices;
};