
// printf 函数声明
void printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));

void main() {
  // 主函数，当前为空
  // 可以在这里添加更多的内核功能
  printf("------------------------\n");
  printf("printf in kernel/main.c\n");
  printf("------------------------\n");

  while (1) {
    // 无限循环，保持内核运行
  }
}
