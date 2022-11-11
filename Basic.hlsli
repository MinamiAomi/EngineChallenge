

float2 Rotated(float2 pos, float theta)
{
    float2 tmp;
    tmp.x = pos.x * cos(theta) - pos.y * sin(theta);
    tmp.y = pos.x * sin(theta) + pos.y * cos(theta);
    return tmp;
}