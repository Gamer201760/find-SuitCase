import eel

@eel.expose()
def send(name, fname, flight):
    print(name)
    print(fname)
    print(flight)

eel.init("web")
eel.start("index.html", size=(500,500))