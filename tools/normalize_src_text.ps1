[CmdletBinding()]
param(
	[string]$Root = (Join-Path $PSScriptRoot "..\src"),
	[switch]$Apply,
	[switch]$IncludeThirdParty,
	[int]$SpacesPerTab = 4
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if ($SpacesPerTab -lt 1) {
	throw "SpacesPerTab must be greater than 0."
}

$resolvedRoot = (Resolve-Path -LiteralPath $Root).Path
$utf8Strict = [System.Text.UTF8Encoding]::new($false, $true)
$utf8NoBom = [System.Text.UTF8Encoding]::new($false)

function Test-BytesEqual {
	param(
		[byte[]]$Left,
		[byte[]]$Right
	)

	if ($Left.Length -ne $Right.Length) {
		return $false
	}

	for ($i = 0; $i -lt $Left.Length; $i++) {
		if ($Left[$i] -ne $Right[$i]) {
			return $false
		}
	}

	return $true
}

function Convert-LeadingIndent {
	param(
		[string]$Line,
		[int]$TabWidth
	)

	if ($Line.Length -eq 0) {
		return $Line
	}

	$indentLength = 0
	while ($indentLength -lt $Line.Length) {
		$ch = $Line[$indentLength]
		if ($ch -ne ' ' -and $ch -ne "`t") {
			break
		}
		$indentLength++
	}

	if ($indentLength -eq 0) {
		return $Line
	}

	$indent = $Line.Substring(0, $indentLength)
	$rest = $Line.Substring($indentLength)
	$builder = [System.Text.StringBuilder]::new()
	$spaceCount = 0

	for ($i = 0; $i -lt $indent.Length; $i++) {
		$ch = $indent[$i]
		if ($ch -eq ' ') {
			$spaceCount++
			if ($spaceCount -eq $TabWidth) {
				[void]$builder.Append("`t")
				$spaceCount = 0
			}
			continue
		}

		if ($spaceCount -gt 0) {
			[void]$builder.Append((' ' * $spaceCount))
			$spaceCount = 0
		}
		[void]$builder.Append($ch)
	}

	if ($spaceCount -gt 0) {
		[void]$builder.Append((' ' * $spaceCount))
	}

	return $builder.ToString() + $rest
}

function Convert-SourceText {
	param(
		[string]$Text,
		[int]$TabWidth
	)

	$normalized = [regex]::Replace($Text, "`r`n|`r|`n", "`n")
	$lines = $normalized.Split([string[]]@("`n"), [System.StringSplitOptions]::None)

	for ($i = 0; $i -lt $lines.Length; $i++) {
		$lines[$i] = Convert-LeadingIndent -Line $lines[$i] -TabWidth $TabWidth
	}

	return [string]::Join("`r`n", $lines)
}

$files = Get-ChildItem -LiteralPath $resolvedRoot -Recurse -File -Include *.cpp,*.h,*.hpp |
	Sort-Object FullName

if (-not $IncludeThirdParty) {
	$thirdPartyRoot = [System.IO.Path]::GetFullPath((Join-Path $resolvedRoot "thirdparty"))
	if (-not $thirdPartyRoot.EndsWith([System.IO.Path]::DirectorySeparatorChar)) {
		$thirdPartyRoot += [System.IO.Path]::DirectorySeparatorChar
	}

	$files = $files | Where-Object {
		-not [System.IO.Path]::GetFullPath($_.FullName).StartsWith($thirdPartyRoot, [System.StringComparison]::OrdinalIgnoreCase)
	}
}

$changed = 0
$skipped = 0

foreach ($file in $files) {
	$bytes = [System.IO.File]::ReadAllBytes($file.FullName)
	try {
		$text = $utf8Strict.GetString($bytes)
	} catch {
		$skipped++
		Write-Warning "Skipped non-UTF-8 file: $($file.FullName)"
		continue
	}

	if ($text.Length -gt 0 -and $text[0] -eq [char]0xfeff) {
		$text = $text.Substring(1)
	}

	$newText = Convert-SourceText -Text $text -TabWidth $SpacesPerTab
	$newBytes = $utf8NoBom.GetBytes($newText)

	if (Test-BytesEqual -Left $bytes -Right $newBytes) {
		continue
	}

	$changed++
	if ($Apply) {
		[System.IO.File]::WriteAllBytes($file.FullName, $newBytes)
		Write-Host "Normalized: $($file.FullName)"
	} else {
		Write-Host "Would normalize: $($file.FullName)"
	}
}

if ($Apply) {
	Write-Host "Done. Normalized $changed file(s). Skipped $skipped file(s)."
} else {
	Write-Host "Dry run complete. $changed file(s) would be normalized. Skipped $skipped file(s)."
	Write-Host "Run with -Apply to rewrite files."
}
