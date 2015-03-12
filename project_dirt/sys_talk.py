from subprocess import Popen, PIPE

def write(s):
    s = s.split()
    out = Popen(s,stdout = PIPE).communicate()[0]
    return out.decode("utf-8")


if __name__ == "__main__":
    print(write("echo Hello"))
