#ifndef CPU_H
#define CPU_H

#define FORMAT_SPECIFIER(type) _Generic((type), \
    int: "%d", \
    float: "%f", \
    double: "%lf", \
    char*: "%s" \
)

#define HANDLE_ERROR(message)                               \
    perror(message);                                        \
    fprintf(LOG_FILE, message);                             \
    fclose(file);                                           \
    return 1;

#define CPU_DUMP(source) CpuDump(source);

#define HANDLE_REGISTER(reg, index)             \
    case reg:                                   \
        return source->codeRegister[index];     \
        break;

int CheckStackSizeForOperation (struct Stack* myStack);
int return_arg (struct Cpu* source, int code);
void ProcesscodeArray (struct Cpu* source);
void PopArg (struct Cpu* source, int code);

// void ArifmeticCommand (struct Cpu* source, int code);

void CommandPrintout (struct Cpu* source);
void PrintBinary (struct Cpu* source);
#endif
