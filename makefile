dev: regenerate-dots regenerate-zips
	URL=http://localhost BASE_URL=/ yarn run start

deploy-aisa:
	URL="https://fi.muni.cz" BASE_URL="~xfocko/kb/" yarn run build
	rsync -avzrlpptv --delete build/ aisa:~/public_html/kb/

deploy-poincare:
	URL="https://blog.mfocko.xyz" BASE_URL="/" yarn run build
	rsync -avzrlpptv --delete build/ poincare:~/public_html/blog/

deploy: regenerate-dots regenerate-zips deploy-aisa deploy-poincare

regenerate-dots:
	sh regenerate-dots.sh

regenerate-zips:
	sh regenerate-zips.sh

.PHONY: deploy-aisa deploy-poincare regenerate-dots regenerate-zips
