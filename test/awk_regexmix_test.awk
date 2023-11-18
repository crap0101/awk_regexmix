
@load "regexmix"

BEGIN {
    split("1", a, ":")
    a[0] = @/^.*?x+9$/
    # a[1] is strnum 1
    a[2] = "foo"
    a[3]
    a[4] = ""
    a[5] = 19
    a[6] = invented
    for (i in a) {
        r = regexmix::make(a[i])
        printf("<%s> (%s) --> <%s> (%s)\n", a[i], typeof(a[i]), r, typeof(r));
    }
    #u = regexmix::make() # fatal
    #printf("<%s> (%s) --> <%s> (%s)\n", a[i], typeof(a[i]), r, typeof(r));
    u = regexmix::make(1, 2)
    printf("too many args --> <%s> (%s)\n", u, typeof(u));
}
