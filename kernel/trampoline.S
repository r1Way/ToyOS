.section trampsec
.globl usertrap
.globl uservec
.globl userret

uservec:
        # 保存上下文
        jal usertrap

userret:
        # 恢复用户态上下文
        # sret