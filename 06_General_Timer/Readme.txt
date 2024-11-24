
1.这个给函数有bug,对于参数Comparel直接赋值是可以使用的，但是定义一个变量传递给他就是不行
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint32_t Compare1);