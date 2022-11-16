#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

// my fns
void print_preferences(void);
void print_pairs(void);
bool pair_exists(int winner, int loser);
bool cycle(int end, int cycle_start);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
            preferences[i][j] = 0;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }
    print_preferences(); // print_preferences

    add_pairs();
    print_pairs(); // print pairs
    sort_pairs();
    print_pairs();
    lock_pairs();

    printf("Locked graph looks like: \n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%d, ", locked[i][j]);
        }
        printf("\n");
    }

    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            // update the ranks array
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // ranks[i] is the voter’s ith preference
    for (int i = 0; i < candidate_count; i++) // candidate_count is the length of ranks array
    {
        // ranks[i] was preffered over ranks[i + 1] and ranks[i + 2] and so on
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    // pairs struct, pair_count var
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i == j) // no need to inspect further
            {
                continue;
            }
            // printf("comparing %i with %i\n", preferences[i][j], preferences[j][i]);
            if (preferences[i][j] > preferences[j][i] && !pair_exists(i, j))
            {
                // printf("I am here\n");
                pairs[pair_count].winner = i;
                pairs[pair_count++].loser = j;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // lets start anew

    int index_array[pair_count];

    int margin = 0;
    for (int i = 0; i < pair_count; i++) // 0, 2
    {
        margin = 0;
        for (int j = 0; j < candidate_count; j++) // 0, 3
        {
            margin = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        }
        index_array[i] = margin;
    }

    for (int i = 0; i < pair_count - 1; i++)
    {
        int most_index = i;
        for (int j = i + 1; j < pair_count; j++)
        {
            // compare j and most_index
            if (index_array[j] > index_array[most_index])
            {
                most_index = j;
            }
        }
        // swap i, most_index
        pair temp = pairs[i];
        pairs[i] = pairs[most_index];
        pairs[most_index] = temp;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loop through pairs
    for (int i = 0; i < pair_count; i++)
    {
        // If cycle function returns false, lock the pair
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // winner has his/her column false in locked 2D array
    for (int i = 0; i < candidate_count; i++)
    {
        bool not_a_winner = false;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                // not a winner
                not_a_winner = true;
            }
        }
        if (!not_a_winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}

// my fns
bool cycle(int end, int cycle_start)
{
    // Return true if there is a cycle created (Recursion base case)
    if (end == cycle_start)
    {
        return true;
    }
    // Loop through candidates (Recursive case)
    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[end][i])
        {
            if (cycle(i, cycle_start))
            {
                return true;
            }
        }
    }
    return false;
}

void print_preferences(void)
{
    printf("preferences array look like this:\n");
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            printf("%d, ", preferences[i][j]);
        }
        printf("\n");
    }
}

void print_pairs(void)
{
    printf("pairs array look like this:\n");
    for (int i = 0; i < pair_count; i++)
    {
        printf("Winner is: %d, loser is: %d\n", pairs[i].winner, pairs[i].loser);
    }
}

bool pair_exists(int winner, int loser)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (pairs[i].winner == winner && pairs[i].loser == loser)
        {
            return true;
        }
    }
    return false;
}