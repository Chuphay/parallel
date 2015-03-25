from lxml import html
import requests

filename = 'text.txt'
target = open('text.txt', 'w')
print("Erasing ", filename) 
target.truncate()

def scrape(seed = "http://stackoverflow.com/questions/15272087/extract-href-values-with-xpath-on-python-2-7", depth = 1, amount = 10):
    print(depth, amount)
    page = requests.get(seed)
    tree = html.fromstring(page.text)
    text = tree.xpath('//p/text()')
    target.write(' '.join(text))

    if((depth>0)):
        for elt in tree.xpath('//a'):

            if(amount<=0):
                break
            try:
                link = elt.attrib['href']
                if(link[:4] == 'http'): 
                    amount = amount - 1
                    scrape(link, depth - 1, amount)

            except KeyError:
                pass
 
scrape()  
