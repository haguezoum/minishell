# ifndef TOKEN_H
# define TOKEN_H
enum e_token {
    WORD = -1,
    WHITE_SPACE = ' ',
    NEW_LINE = '\n',
    QUOTE = '\'',
    DOUBLE_QUOTE = '\"',
    ESCAPE = '\\',
    ENV = '$',
    PIPE = '|',
    REDIR_IN = '<',
    REDIR_OUT = '>',
    HERE_DOC,
    DREDIR_OUT
};





#endif
