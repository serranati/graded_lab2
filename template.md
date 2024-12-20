---
title: The Bad Program
author: Ikrame Rekabi and Natalia Serra Antonucci
date: 27 November 2024
geometry: top=1.5cm,left=2cm,right=2cm,bottom=2cm
license: CC0 1.0
numbersections: true
highlightstyle: pygments
header-includes:
    - \usepackage{package_deps/comment}
    - \usepackage{multicol}
    - \usepackage{lmodern}
    - \newcommand{\hideFromPandoc}[1]{#1}
    - \hideFromPandoc{
        \let\Begin\begin
        \let\End\end
      }
colorlinks: true
lang: en-GB
---

<!-- Remplacer 'en-GB' par 'fr' pour un document en franÃ§ais -->

<!-- 
	Markdown is a minimalistic HTML-based language
	
	The restricted set of functionalities is BY DESIGN,
	It is useful in any course context to restrict report formatting.
	You SHALL NOT try to circumvent how it works. You must CONFORM to the format. 
	Any text editor will work, since Markdown is just plain text.
	
	Visual Studio Code has nice basic support and previewing capabilities,
	and support several extensions that enhance the writing experience.
	You are stroongly advised to use them.

	The template is commented to guide you through the format,
	but you should *read* the comments to understand.

	Note that line breaks do not exist. You cannot skip a line, 
-->

<!-- Sections are denoted with a "#" at the very beginning of the line. Do not number the sections! Pandoc will handle that part -->
# Introduction

This project is based on a vulnerable door locker program that was exploited in the previous exercise. This program asks the user to input two parameters that have to be equal to allow access, or, open the door. However, the program had many vulnerabilities that allows many threats to be exploited. The threats identified are:

- Exploitation through buffer overflow due to the lack of input data sanitization. It includes the lack of bounds checking, type checking and fixed allocation of the buffer in the `validate()` function.
- The use of weak functions regarding security such as `scanf()`.
- The use of a hidden functon that grants root access even though it is not called in the `main()` (`fnR()`).

<!-- Each new "#" denote a lower section level, "##": subsection; "###": subsubsection; and so on. -->
# Legacy options

The first part of this part of the laboratory was to find the compilation and linker flags (legacy options) used in the Makefile to create the door-locker program exploited in the previous project. Legacy options in a Makefile for a C program are ways of specifying rules, variables, or options in the building process of the program. The flags chosen where guided from what was mentioned in the analysis of exploitation and mitigation techniques of the previous laboratory. 

The `LEGCFLAGS` defined in the Makefile are options added to the compiler, or the compilation stage. `-m32` was chosen as it defines a 32-bit architecture, which is the type of system aspect that allows us to exploit the program. `-fno-stack-protector` was also added as the program did not have a canary to detect a buffer overflow attempt. `-D_FORTIFY_SOURCE=0` was added as no optimizations for buffer overflow detection seemed to be enabled for the program in the previous laboratory. Lastly, `-fPIE` is enabled as it us a key factor for ASLR, which was also enabled in the previous program and had to be disabled to be able to exploit it.

On the other hand, the `LEGLDFLAGS` are options added to manipulate the linker or linking stage. In this case, `-m32` was also added to fully make the program a 32-bit architecture; if it was only specified in `LEGCFLAGS`, it would throw an error stating that the `.c` files do not match the architecture of the compiler. The second flag was `-pie` to enable the Position Independent Execution previously mentioned, as it is crucial for ASLR.

## Acceptable legacy options

Some flags proposed enabled security conditions on the program, while other ones disabled them. 

### Flags that should be removed or replaced

The architecture of the `.c` files for the program is of 64-bit, which allows for more security optimizations (e.g., kernel patch protection). When the exploitation was tested on a 64-bit architecture, we could not perform the buffer overflow as the memory addresses had unprintable characters that did not permit our exploit. For this reason, the `-m32` flag should be removed. 

`-fstack-protector` is a flag allowing the creation of a canary at compilation to alarm the system when there is an attempt of buffer overflow. It enables stack protection for functions that handle arrays with certain characteristics, like size. In the previous program, the flag used has the opposite effect, disabling this security feature. Therefore, `-fno-stack-protector` should be replaced with `-fstack-protector` or even `-fstack-protector-strong`, which has a more strict nature for protection as it considers all functions handling arrays, no matter the size.

`-D_FORTIFY_SOURCE` should be set to 2 rather than 0, as 0 disables security optimizations that could detect buffer overflow attempts. Setting `-D_FORTIFY_SOURCE=2` allows the compiler to recognize string handling functions that could be vulnerable to exploitations. `-D_FORTIFY_SOURCE=3` can also be used as it makes a more thorough analysis of the use of string functions but it has a bigger impact on performance.

### Flags that should be kept or added

The Position Independent Execution (PIE) option should be kept as it enables ASLR. This is a crucial security function that randomizes memory addresses of the functions of the program (ASLR) and the program itself (PIE). This aspect restricts the replacement of memory addresses by maliciously constructed functions or pointing to functions that allow some elevation of privilege, or that represent any other threat to the system.

For the program presented, it is not necessary to make the stack executable. Furthermore, including the flag `-z noexecstack` is a great decision as it does not affect the intended functioning of the program and it protects it from malicious code injections.

# Problematic lines in the code
The problematic lines in the code are primarily those that declare the buffer with a size of 20, even though we only expect single-character inputs like 'Y' or 'n'.

Another issue is the use of scanf without proper bounds checking, which can lead to buffer overflow vulnerabilities. Additionally, the visibility of the 'fnR' function could be problematic, as it might not be properly scoped or declared where needed.

Lastly, another issue is the handling of the first user input without any type checking, which can result in incorrect behavior if the input is not as expected.

# The patched vulnerabilities

First of all, we added the necessary flags to the Makefile.

Regarding the buffer size, we decided to set it to 3 instead of 20, as only a single character (such as 'Y' or 'n') is required. While we could have dynamically allocated the buffer, it is unnecessary since the input size will remain fixed.

For bounds checking, we switched from `scanf()` to using `fgets()`, which automatically limits input based on the buffer size. This ensures that the user cannot overwrite the buffer, preventing potential buffer overflows.

To address the visibility of the fnR function when using debugging tools, we added the '#ifndef DEBUG_MODE' directive, that is used as part of conditional compilation. It checks whether a specific macro (in this case, DEBUG_MODE) has not been defined before compiling the code that follows it. If `DEBUG_MODE` is not defined, the code between `#ifndef DEBUG_MODE` and `#endif` will be included in the compilation process. If `DEBUG_MODE` is defined, that code is excluded. This prevents access to the function when debugging is enabled.

Lastly, for handling the first input, we implemented a function that checks the type of the input and ensures only numeric values are accepted.

# Future considerations

The door locker program is a system that should be implemented with security aspects in mind as it allows or restricts access. It was mentioned in the previous laboratory that the password is weak, and it can be deciphered from the decompiled or the source code. The first consideration for future implementations would be to have a more complicated or hidden password. This can be achieved by encrypting it in a separate file that is loaded in the `main()` function. However, that was not the focus of this project, rather it was oriented towards the functionality, exploitability and manipulation of the program.

Input sanitization is a must because it can lead to vulnerabilities that can be exploited through buffer overflow attacks (like the presented in these laboratories), or even malicious code injections. User input should always be checked in both format and length. This can be achieved by implementing functions like `fgets()` instead of `scanf()`, where these characteristics can be validated. This extends to functions handling memory, where it is preferable to use functions that also consider memory safety and do not impact performance  (e.g., use `strncpy()` instead of `strcpy`). 

Regarding the `fnR()` function and root privileges, the Least Privilege Principle should also be considered. If the program does not require it, access to executing it with elevated privileges should be restricted, as it can lead to risk escalation. Root privileges should only be granted if necessary for the program's functionality and purpose.

Other compilation options can be considered to be included. The `relro` flag stands for Relocation Read-Only which does not allow to overwrite the locations (memory addresses) of the functions, making these read-only objects. This would disable part of the attack proposed in the previous laboratory where an address is replaced with the memory address of the `fnR()` function to gain root privileges for the execution of the program. Another compilation option is `-fsanitize=bounds` which enables the detection of out-of-bounds array accesses to recognize memory violations at runtime.

Authentification and Authorization are security requirements that are really important for the implementation of such program. The consideration of these requirements would allow more access control over the usage of the program and avoid adverts from tampering with the system. Finally, a great suggestion for tracking accesses to the program is logging. This would include having a response for critical events (e.g., access attempts) and monitoring all accesses to the system to determine suspicious activies.

# Conclusions

The analysis and modifications to the vulnerable door locker program really show that security should not be taken lightly in any access control system. The program initially had critical vulnerabilities such as lack of input sanitization, use of insecure functions, and exposing sensitive functions like `fnR()` to any user. These vulnerabilities made it prone to buffer overflow attacks and privilege escalation, showing the need for secure coding practices and system analysis.

To fix these, legacy compilation and linker options that disabled security features were replaced with their secure opposites. For example, enabling stack protection with `-fstack-protector-strong`, buffer overflow detection with `-D_FORTIFY_SOURCE=2`, and memory address randomization through `-fPIE` and `-pie` greatly hardened the program compared to the initial options. Additional flags like `-z noexecstack` and `relro` were included in the report as measures to prevent code injection and overwriting of function addresses.

Another important improvement was input validation. Replacing insecure functions like `scanf()` with their safer counterparts, such as `fgets()`, ensured that user inputs were well sanitized for format and length to avoid buffer overflows and execution of malicious code. Only allowing numeric values and adjusting the buffer size contributed to the input validation as well. The principle of least privilege was discussed in the restriction of root-level access, ensuring sensitive functions like `fnR()` were secured through conditional compilation or excluded from debugging builds.

Future development should focus on encrypting sensitive information, improving authentication and authorization mechanisms, and including runtime security checks such as `-fsanitize=bounds`. Keeping libraries and dependencies up to date with regular testing is also necessary for the security of the program. To conclude, by following these best practices, the door locker program can become a more secure application that will prevent traditional exploitations.



```
