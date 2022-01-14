import json
import requests

url = 'http://ipinfo.io/json'
response = requests(url)
data = json.load(response)

IP = data['ip']
org = data['org']
city = data['city']
country = data['country']
region = data['region']

print(f"{IP} {city}")
