import requests
from bs4 import BeautifulSoup

response = requests.get("https://habr.com/ru/post/1/")
response.status_code

soup = BeautifulSoup(response.text, "html.parser")

author_uri = soup.find(class_="tm-user-info__username").get("href")
author = a.get_text().strip()

article_text = str(soup.find(id="post-content-body").get_text())
title = soup.find(class_="tm-article-snippet__title tm-article-snippet__title_h1").get_text()
timestamp = soup.find(class_="tm-article-snippet__datetime-published").find("time").get("datetime")

# hubs
for hub in soup.find(class_="tm-article-snippet__hubs"):
    h = hub.find(class_="tm-article-snippet__hubs-item-link")
    print(h.get("href"))
    print(h.get_text())
# tags

for child in soup.find(
    class_="tm-separated-list tm-article-presenter__meta-list"
).find(
    class_="tm-separated-list__list"
):
    print(child.get_text())