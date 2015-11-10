.globl switch_task
switch_task:
  mov 4(%esp), %eax
  mov 8(%esp), %edx

  # Сохраним старые регистры
  push %ebp
  push %ebx
  push %esi
  push %edi
  pushfl

  # Перекинем стеки
  mov %esp, (%eax)
  mov %edx, %esp

  # Загрузим новые регистры
  popfl
  pop %edi
  pop %esi
  pop %ebx
  pop %ebp

  ret
