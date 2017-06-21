echo "starting build ==============================="
cd docs
bundle install
echo "bundle installed ============================="
bundle exec jekyll build
echo "ending build ================================="
