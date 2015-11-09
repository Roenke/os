.global switch_task
switch_task:
  mov 4(%esp), %eax
  mov 8(%esp), %edx

  # save old registers
  push %ebp
  push %ebx
  push %esi
  push %edi
  # pushf
  # switch stacks
  mov %esp, (%eax)
  mov %edx, %esp

  # load new registers
  # popf
  pop %edi
  pop %esi
  pop %ebx
  pop %ebp

  sti
  ret
