echo "starting build ==============================="
cd docs
ls -l
bundle install
echo "bundle installed ============================="
bundle exec jekyll build
echo "ending build ================================="
