/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychufist <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/18 14:24:19 by ychufist          #+#    #+#             */
/*   Updated: 2018/12/18 14:25:28 by ychufist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

int is_answer(t_farm farm, char *line, int id)
{
    int j;
    t_list_room *room_list;
    t_list_room *dop2;
    t_list_room *dop;

    room_list = ft_list_room_new(farm.rooms[farm.start_room_id]);
    dop = room_list;
    dop2 = room_list;;
    while (dop2)
    {
        if (ft_strcmp(dop2->name, "end") == 0)
        {
            print_list(dop);
            return (1);
        }
        j = -1;
        while (++j < dop2->links_amount)
        {
            if (!ft_list_room_find(dop, dop2->links[j]))
            {
                room_list->next = ft_list_room_new(farm.rooms[is_room(farm, dop2->links[j])]);
                room_list = room_list->next;
            }
        }
        dop2 = dop2->next;
    }
//    print_list(dop2);
    ft_printf("NO WAY TO END :(\n");
    return (0);
}

//void *no_traffic_jams(t_farm farm)
//{
//    int i;
//    int j;
//    t_list_room **way;
//
//    i = 0;
//    j = 0;
//    way = (t_list_room**)malloc(sizeof(t_list_room*) *
//            farm.rooms[farm.end_room_id].links_amount);//ft_list_room_new(farm.rooms[farm.end_room_id]);
//    while (i < farm.rooms[farm.end_room_id].links_amount)
//    {
//        way[i] = ft_list_room_new(farm.rooms[is_room(farm,
//                                             farm.rooms[farm.end_room_id].links[i])]);
//        farm.rooms[is_room(farm,
//                           farm.rooms[farm.end_room_id].links[i])].flag = 1;
//        j = farm.rooms[is_room(farm, farm.rooms[farm.end_room_id].links[i])];
//        while (j < )
//        print_list(way[i]);
//        i++;
//    }
//    while (dop)
//    {
//
//        dop = dop->next;
//    }
//}

int is_valid_name(const char *name)
{
    if (name[0] == 'L' || name[0] == '#')
    {
        ft_printf("NAME ERROR\n");
        return (0);
    }
    return (1);
}

int is_room(t_farm farm, char *name)
{
    int i;

    i = 0;
    while (i < farm.room_amount)
    {
        if (ft_strcmp(farm.rooms[i].name, name) == 0)
            return (i);
        i++;
    }
    return (-1);
}

int find_link(t_farm *farm, char *line, int k)
{
    t_point ij;
    char *dop;
    char *name;

    ij.y = -1;
    dop = ft_strchr(line, '-');
    name = ft_strsub(line, 0, dop - line);
    dop++;
    ij.x = is_room(*farm, name);
    if (ij.x > -1 && is_room(*farm, dop) > -1)
    {
        while (++ij.y < farm->rooms[ij.x].links_amount)
            if (ft_strcmp(farm->rooms[ij.x].links[ij.y], dop) == 0)
                return (1);
        if ((ij.y == farm->rooms[ij.x].links_amount ||
            ft_strlen(farm->rooms[ij.x].links[ij.y]) == 0))
            add_link(farm, dop, ij.x);
        if (++k < 2)
        {
            dop = ft_strcat(ft_strcat(dop, "-"), name);
            find_link(farm, dop, k);
        }
        ft_strdel(&name);
        return (1);
    }
    return (0);
}

int get_info(t_farm *farm, char *line, int *i)
{
    char *dop;

    if ((*i)++ == 0)
        farm->ants_amount = ft_atoi(line);
    else if (ft_strchr(line, '-') == NULL && farm->flag == 0)
    {
        add_room(farm);
        dop = ft_strchr(line, ' ');
        farm->rooms[farm->room_amount].name = ft_strsub(line, 0, dop - line);
        if (!is_valid_name(farm->rooms[farm->room_amount].name))
            return (0);
        farm->rooms[farm->room_amount].coord.x = ft_atoi(dop++);
        farm->rooms[farm->room_amount].coord.y = ft_atoi(ft_strchr(dop, ' '));
        farm->rooms[farm->room_amount++].links_amount = 0;
    }
    else if (ft_strchr(line, '-') != NULL && farm->flag == 0)
    {
        if (!find_link(farm, line, 0))
        {
            ft_printf("\n\nNO SUCH ROOM OR LINK. ERROR\n");
            return (0);
        }
    }
    return (1);
}

int main(void) {
	int i;
	int fd;
	char *line;
	t_farm farm;
	t_list_room *way;

	i = 0;
	fd = open("/Users/ychufist/lem-in/test", O_RDONLY);//("/home/echufy/lem-in/test", O_RDONLY);
	line = NULL;
	farm.flag = 0;
	farm.room_amount = 0;
	farm.rooms = (t_room *) malloc(sizeof(t_room) * (farm.room_amount + 1));
	while (get_next_line(fd, &line) > 0)
	{
		ft_printf("gnl : %s\n", line);
		if (line[0] != '#')
		{
			if (!get_info(&farm, line, &i))
				return (0);
		}
		else if (ft_strstr(line, "start") != NULL)
			farm.start_room_id = farm.room_amount;
		else if (ft_strstr(line, "end") != NULL)
			farm.end_room_id = farm.room_amount;
		ft_strdel(&line);
	}

	i = 0;
	ft_printf("room amount %d\n", farm.room_amount);
    while (i < farm.room_amount)
    {
	//write(1, "\e[34m", 5);
	ft_printf("%d) ", i);
	ft_printf("room name %s ", farm.rooms[i].name);
	ft_printf("room coord x %d ", farm.rooms[i].coord.x);
	ft_printf("room coord y %d ", farm.rooms[i].coord.y);
	ft_printf("links amount %d\n", farm.rooms[i].links_amount);
	i++;
    }
	//write(1, "\e[0m", 4);
	i = 0;
	while (i < farm.rooms[4].links_amount) {
		ft_printf("%s-%s\n", farm.rooms[4].name, farm.rooms[4].links[i]);
		i++;
	}
	if (is_answer(farm, "end", farm.start_room_id))
    {
//	    no_traffic_jams(farm);
        //way =
        //print_list(way);
    }
	ft_printf("\n\nokk");
	return (0);
}