void free_file_struct(t_file **p)
{
	t_file * file_struct;

	if (!p || !*p)
		return;
	file_struct = *p;
	if (file_struct->name)
		free(file_struct->name);
	if(file_struct->sep)
		free(file_struct->sep);
	free(file_struct);
	*p = NULL;
}

void call_free_fs(void *p)
{
	t_file **file;

	file = (t_file **)p;
	free_file_struct(file);
}

void	ft_lstfree(t_list **lst,void free_foo(void *))
{
	t_list	*tmp;

	while (lst && *lst)
	{
		tmp = *lst;
		*lst = (*lst)->next;
		if (free_foo)
			free_foo(&(tmp->content));
		ft_lstdelone(tmp);
	}
	lst = NULL;
}
void free_cmd_value(t_cmd_value *val)
{
	free(val->value);
	free_tab(val->args);
	if (val->path)
	{
		val->path =NULL;	
		free(val->path);
	}
	if (val->env)
		free_tab(val->env);
	val->value = NULL;
	val->args = NULL;
	val->path = NULL;
	val->env = NULL;
}



void free_red(t_redirect *red)
{
	ft_lstfree(&(red->out_list), call_free_fs);
	ft_lstfree(&(red->in_list), call_free_fs);
}
//attention ne free pas les commande linker
void free_cmd(t_cmd **p)
{
	t_cmd *cmd;
	if (!p)
		return;
	cmd = *p;
	if (!cmd)
	{
		*p = NULL;
		return;
	}
	free_cmd_value(&(cmd->val));
	free_red(&(cmd->red));	
	
	free(cmd);
	*p = NULL;
}

void free_common(t_cmd*cmd)
{
	if (!cmd)
		return;
	ft_lstfree(&(cmd->export_history), free);	
	ht_del_hash_table(cmd->env);	
}

void rec_free_commands(t_cmd *cmd)
{
	t_cmd *next;
	t_cmd *red;
	
	next = cmd->next;
	red = cmd->red.next_cmd;
	if (!red && !next)
		return free_cmd(&cmd);
	rec_free_commands(next);
	rec_free_commands(red);
}

void free_commands(t_cmd *cmd, bool common)
{
	if (common)
		free_common(cmd);
	rec_free_commands(cmd);
}