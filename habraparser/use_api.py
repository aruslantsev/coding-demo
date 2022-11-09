import json
import requests

USER_AGENT="Mozilla/5.0 (X11; Linux x86_64; rv:106.0) Gecko/20100101 Firefox/106.0"

# get last post
url = "https://m.habr.com/kek/v2/articles/all"
headers = {'User-Agent': USER_AGENT}
params = {
    "date": "day",
    "sort": "date",
}

r = requests.get(url, headers=headers, params=params)
# check r.status_code
data = json.loads(r.text)
last_post_id = data['articleIds'][0]

# get post by id
url = "https://m.habr.com/kek/v2/articles/{}"
headers = {'User-Agent': USER_AGENT}

r = requests.get(url.format(686672))

data = json.loads(r.text) # dict with all data

# get comments by id
url = "https://m.habr.com/kek/v2/articles/{}/comments"
headers = {'User-Agent': USER_AGENT}

r = requests.get(url.format(686672))

data = json.loads(r.text) # dict with all data