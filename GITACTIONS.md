# Git actions
Pull steps:
1. git init
2. git pull git@github.com:rodapunk/LearnOpenGL.git

First push steps:
1. git add .
2. git commit -m "Message"
3. git branch -M main
4. git remote add origin git@github.com:rodapunk/LearnOpenGL.git
5. git push -u origin main
  
Push steps:
1. git add .
2. git commit -m "Message"
3. git push

Change last commit after push:
1. git commit --amend -m "New message"

Change more than one commit after push:
1. git rebase -i HEAD~x (x -> number of commits)
2. press i and change the word 'pick' to 'reword', after leave with esc : w q
3. press i to edit message, after leave with esc : w q
4. git push --force-with-lease

Git push with force:
1. git push origin master -f
