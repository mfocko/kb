dev:
	URL=http://localhost BASE_URL=/ npm run start

deploy-aisa:
	URL=https://fi.muni.cz BASE_URL=~xfocko/ib002/ npm run build
	rsync -avzrlpptv --delete build/ aisa:~/public_html/ib002/

deploy-poincare:
	URL=https://fi.mfocko.xyz BASE_URL=/ npm run build
	rsync -avzrlpptv --delete build/ poincare:~/public_html/fi/

.PHONY: deploy-aisa deploy-poincare
