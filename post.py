import requests

files = {
    'pic1': ("bag1.png",open('./photo/bag1.png', 'rb'), 'image/png'),
    'pic2': ("bag2.png",open('./photo/bag2.png', 'rb'), 'image/png'),
    'pic3': ("bag3.png",open('./photo/bag3.png', 'rb'), 'image/png'),
    'pic4': ("bag4.png",open('./photo/bag4.png', 'rb'), 'image/png')
}
data = {
    "name": "Azamat",
    "f_name": "Nuraev",
    "flight": "Su8943"
}
code=requests.post('http://localhost/upload', files=files, data=data)   
print(code)