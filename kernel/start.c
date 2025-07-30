// 函数声明，定义在main.c中
void main();

/**
 * 内核启动函数
 * 
 * 这个函数由汇编代码 entry.S 调用
 *
 * 调用链：
 * _entry (entry.S) -> start() (start.c) -> main() (main.c)
 */
void start() {
  main();
}
