from os import system
import requests
import sys

def wind_formatter(wind_speed):
    if wind_speed == 1:
        return ['Below 0.3m/s', 'calm']
    if wind_speed == 2:
        return ['0.3-3.4m/s', 'light']
    if wind_speed == 3:
        return ['3.4-8.0m/s', 'moderate']
    if wind_speed == 4:
        return ['8.0-10.8m/s', 'fresh']
    if wind_speed == 5:
        return ['10.8-17.2m/s', 'strong']
    if wind_speed == 6:
        return ['17.2-24.5m/s', 'gale']
    if wind_speed == 7:
        return ['24.5-32.6m/s', 'storm']
    if wind_speed == 8:
        return ['Over 32.6m/s', 'hurricane']


def main():
    args = sys.argv
    lon = 82.933952
    lat = 55.018803       # Novosibirsk by default
    city = 'Novosibirsk'
    if len(args) > 1:
        city = args[1].capitalize()
        if args[1].lower() == 'moscow':
            lon = 37.618423
            lat = 55.751244
        elif 'peter' in args[1].lower():
            city = 'St.Petersburg'
            lon = 30.308611
            lat = 59.937500
        elif args[1].lower() == 'berdsk':
            lon = 83.086037
            lat = 54.766323
        else: 
            city = 'Novosibirsk'
    
    params = {
        'lon': lon,
        'lat': lat,
        'product': 'astro',
        'output': 'json',
    }
    r = requests.get("http://www.7timer.info/bin/api.pl", params=params, verify=False)
    r.raise_for_status()
    temp = r.json()['dataseries'][0]['temp2m']
    wind = r.json()['dataseries'][0]['wind10m']
    wind_info = wind_formatter(wind['speed'])
    system('clear')
    print(f'Weather in {city} now: {temp}°C, wind: {wind_info[1]} {wind_info[0]}')


if __name__ == '__main__':
    main()