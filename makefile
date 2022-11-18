dev:
	URL=http://localhost BASE_URL=/ npm run start

deploy-aisa:
	URL=https://fi.muni.cz BASE_URL=~xfocko/kb/ npm run build
	rsync -avzrlpptv --delete build/ aisa:~/public_html/kb/

deploy-poincare:
	URL=https://fi.mfocko.xyz BASE_URL=/ npm run build
	rsync -avzrlpptv --delete build/ poincare:~/public_html/fi/

deploy: deploy-aisa deploy-poincare

regenerate-dots:
	bash regenerate-dots.sh

.PHONY: deploy-aisa deploy-poincare regenerate-dots
