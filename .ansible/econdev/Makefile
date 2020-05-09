#
#
#
ansible_playbook=ansible-playbook
playbook=playbook.yml
vault_password_file=vault.txt
cmd=$(ansible_playbook) $(playbook)


all:
	$(cmd) 
	#--vault-id @prompt


#
# Rules for roles
#
%:
	$(cmd) --tags=$*



#
# Standard rules
#

new:
	@if [ "$(param1)" = "" ]; then\
		echo -e "usage:\n  make new param1=<rolename>";\
	else\
		echo "Creating new roles/$(param1)";\
		cp -ir ../.templates/role roles/$(param1);\
	fi


# vim:ts=2:sw=2:noexpandtab
