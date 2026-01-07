using System.Net;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using dotenv.net;
using dotenv.net.Utilities;
using static System.TimeSpan;
using Spectre.Console;

namespace STRSTeamsCheckIn;

internal static class Program
{
    private static bool TryCreateEnvFile(string envFilePath)
    {
        try
        {
            File.WriteAllText(envFilePath, "TOKEN=PASTE_YOUR_TOKEN");

            // Set restrictive permissions on Unix-like systems
            if (!OperatingSystem.IsWindows())
                File.SetUnixFileMode(envFilePath, UnixFileMode.UserRead | UnixFileMode.UserWrite);

            Console.WriteLine($"Created env file. Add your token in {envFilePath} and restart the program.");
            return true;
        }
        catch (Exception ex)
        {
            Console.WriteLine($"Failed to create .env file: {ex.Message}");
            return false;
        }
    }

    private static bool TryGetToken(string envFilePath, out string outToken)
    {
        outToken = "";

        if (!File.Exists(envFilePath))
        {
            TryCreateEnvFile(envFilePath);
            return false;
        }

        DotEnv.Load(options: new DotEnvOptions(envFilePaths: [envFilePath]));

        if (!EnvReader.TryGetStringValue("TOKEN", out var token))
        {
            Console.WriteLine("TOKEN not found in .env file.");
            Console.WriteLine("Recreate .env file? (overwrites existing file)");
            Console.Write("(y/n): ");

            var input = Console.ReadLine()?.Trim().ToLower();

            const string manualGuidance =
                "Manually add TOKEN=your_token_here to .env and restart OR run the program and select re-create the file.";

            switch (input)
            {
                case "y":
                    TryCreateEnvFile(envFilePath);
                    return false;
                case "n":
                    Console.WriteLine("File not created.");
                    Console.WriteLine(manualGuidance);
                    return false;
                default:
                    Console.WriteLine("Invalid input.");
                    Console.WriteLine("File not created.");
                    Console.WriteLine(manualGuidance);
                    return false;
            }
        }

        if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
        {
            var panel = new Panel(
                    $"Add your token to:\n[cyan]{envFilePath}[/]")
                .Header("Setup Required")
                .BorderColor(Color.Yellow);

            AnsiConsole.Write(panel);
            return false;
        }

        outToken = token;
        return true;
    }

    private static async Task Main()
    {
        AnsiConsole.Write(new Rule($"[{AppColors.StrsYellow} bold]STRS Teams Check In[/]")
        {
            Style = Style.Parse(AppColors.StrsBlue),
            Justification = Justify.Center,
        });

        if (!TryGetToken(Path.Combine(AppContext.BaseDirectory, ".env"), out var token))
            return;

        using var client = new TeamsClient(token);

        string location;
        while (true)
        {
            Console.Write("Where are you?: ");
            location = Console.ReadLine() ?? string.Empty;

            if (!string.IsNullOrWhiteSpace(location))
                break;
        }

        var (success, response, errorMessage) = await client.CheckIn(location);

        if (!success)
        {
            Console.WriteLine($"[Error] - {errorMessage}");
            return;
        }

        if (response == null)
        {
            Console.WriteLine("[Error] - response is null.");
            return;
        }

        if (response.StatusCode == HttpStatusCode.NoContent)
        {
            Console.WriteLine("Not Signed In.");
            Console.WriteLine("Possible Reasons:");
            Console.WriteLine("\t- Not on school wifi.");
            Console.WriteLine("\t- Incorrect token.");
            return;
        }

        var content = await response.Content.ReadAsStringAsync();

        var cleanValue = JsonSerializer.Deserialize<string>(content)?.Trim('"') ?? content;

        Console.WriteLine($"[Teams] - {cleanValue}");
    }
}

internal class TeamsClient : IDisposable
{
    private readonly HttpClient _httpClient;
    private bool _disposed;

    public TeamsClient(string token)
    {
        _httpClient = new HttpClient();

        _httpClient.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
        _httpClient.Timeout = FromSeconds(30);
        _httpClient.DefaultRequestHeaders.Add("token", token);
    }

    public async Task<(bool Success, HttpResponseMessage? Response, string? ErrorMessage)> CheckIn(
        string location)
    {
        const string baseUrl = "https://teamsapps.strschool.co.uk/api/touchreg/mobileCheckIn/";
        var locationCode = Convert.ToBase64String(Encoding.UTF8.GetBytes(location))
            .Replace('+', '-')
            .Replace('/', '_'); // Make it URL safe.

        var url = baseUrl + locationCode;

        try
        {
            var response = await _httpClient.PostAsync(url, null);
            return (true, response, null);
        }
        catch (HttpRequestException)
        {
            return (false, null, "Network connectivity issue.");
        }
        catch (OperationCanceledException)
        {
            return (false, null, "Request timed out.");
        }
    }

    public void Dispose()
    {
        if (_disposed) return;

        _httpClient.Dispose();
        _disposed = true;
    }
}

public static class AppColors
{
    public const string StrsBlue = "#234A8C";
    public const string StrsYellow = "#F7E039";
    public const string StrsRed = "#E63B1E";
    public const string Error = "#EF4444";
    public const string Warning = "#E63B1E";
}