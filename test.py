import grequests

urls = ['http://localhost:8000/test', 'http://localhost:8000/test']


params = {'a':'b' * 10000, 'c':'d' * 10000}
rs = (grequests.post(u, data=params) for u in urls)
grequests.map(rs)
