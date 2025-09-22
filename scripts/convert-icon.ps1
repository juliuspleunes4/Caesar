# PNG to ICO Converter for Caesar Icon
# Converts the VS Code extension PNG icon to Windows ICO format

Add-Type -AssemblyName System.Drawing

function Convert-PngToIco {
    param(
        [string]$PngPath,
        [string]$IcoPath
    )
    
    try {
        Write-Host "Converting PNG to ICO..." -ForegroundColor Yellow
        Write-Host "  Source: $PngPath" -ForegroundColor Cyan
        Write-Host "  Target: $IcoPath" -ForegroundColor Cyan
        
        # Load the PNG image
        $png = [System.Drawing.Image]::FromFile($PngPath)
        
        # Create different sizes for the ICO (Windows standard sizes)
        $sizes = @(16, 24, 32, 48, 64, 128, 256)
        $iconBitmaps = @()
        
        foreach ($size in $sizes) {
            # Create a new bitmap with the target size
            $bitmap = New-Object System.Drawing.Bitmap($size, $size)
            $graphics = [System.Drawing.Graphics]::FromImage($bitmap)
            
            # Set high quality rendering
            $graphics.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
            $graphics.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
            $graphics.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
            
            # Draw the PNG scaled to the target size
            $graphics.DrawImage($png, 0, 0, $size, $size)
            $graphics.Dispose()
            
            $iconBitmaps += $bitmap
        }
        
        # Create the ICO file
        $iconStream = New-Object System.IO.FileStream($IcoPath, [System.IO.FileMode]::Create)
        
        # ICO file header (6 bytes)
        $iconStream.WriteByte(0)  # Reserved
        $iconStream.WriteByte(0)  # Reserved
        $iconStream.WriteByte(1)  # Type (1 = ICO)
        $iconStream.WriteByte(0)  # Type high byte
        $iconStream.WriteByte($iconBitmaps.Count)  # Number of images
        $iconStream.WriteByte(0)  # Count high byte
        
        # Calculate offset for image data
        $imageDataOffset = 6 + ($iconBitmaps.Count * 16)  # Header + directory entries
        
        # Write directory entries (16 bytes each)
        foreach ($bitmap in $iconBitmaps) {
            $width = if ($bitmap.Width -eq 256) { 0 } else { $bitmap.Width }
            $height = if ($bitmap.Height -eq 256) { 0 } else { $bitmap.Height }
            
            # Save bitmap to memory to get size
            $memoryStream = New-Object System.IO.MemoryStream
            $bitmap.Save($memoryStream, [System.Drawing.Imaging.ImageFormat]::Png)
            $imageSize = $memoryStream.Length
            $imageData = $memoryStream.ToArray()
            $memoryStream.Dispose()
            
            # Write directory entry
            $iconStream.WriteByte($width)          # Width
            $iconStream.WriteByte($height)         # Height
            $iconStream.WriteByte(0)               # Color count (0 = no palette)
            $iconStream.WriteByte(0)               # Reserved
            $iconStream.WriteByte(1)               # Color planes (low byte)
            $iconStream.WriteByte(0)               # Color planes (high byte)
            $iconStream.WriteByte(32)              # Bits per pixel (low byte)
            $iconStream.WriteByte(0)               # Bits per pixel (high byte)
            
            # Image size (4 bytes, little endian)
            $iconStream.WriteByte($imageSize -band 0xFF)
            $iconStream.WriteByte(($imageSize -shr 8) -band 0xFF)
            $iconStream.WriteByte(($imageSize -shr 16) -band 0xFF)
            $iconStream.WriteByte(($imageSize -shr 24) -band 0xFF)
            
            # Image offset (4 bytes, little endian)
            $iconStream.WriteByte($imageDataOffset -band 0xFF)
            $iconStream.WriteByte(($imageDataOffset -shr 8) -band 0xFF)
            $iconStream.WriteByte(($imageDataOffset -shr 16) -band 0xFF)
            $iconStream.WriteByte(($imageDataOffset -shr 24) -band 0xFF)
            
            $imageDataOffset += $imageSize
        }
        
        # Write image data
        foreach ($bitmap in $iconBitmaps) {
            $memoryStream = New-Object System.IO.MemoryStream
            $bitmap.Save($memoryStream, [System.Drawing.Imaging.ImageFormat]::Png)
            $imageData = $memoryStream.ToArray()
            $iconStream.Write($imageData, 0, $imageData.Length)
            $memoryStream.Dispose()
            $bitmap.Dispose()
        }
        
        $iconStream.Close()
        $png.Dispose()
        
        Write-Host "✅ Successfully converted PNG to ICO" -ForegroundColor Green
        Write-Host "  Created: $IcoPath" -ForegroundColor Green
        
        return $true
        
    } catch {
        Write-Error "Failed to convert PNG to ICO: $($_.Exception.Message)"
        return $false
    }
}

# Main execution
$PngPath = "vscode-extension\images\caesar-icon.png"
$IcoPath = "assets\caesar-icon.ico"

# Create assets directory if it doesn't exist
$AssetsDir = "assets"
if (-not (Test-Path $AssetsDir)) {
    New-Item -ItemType Directory -Path $AssetsDir -Force | Out-Null
    Write-Host "Created assets directory" -ForegroundColor Green
}

# Convert the icon
if (Test-Path $PngPath) {
    $success = Convert-PngToIco -PngPath (Resolve-Path $PngPath).Path -IcoPath $IcoPath
    
    if ($success) {
        Write-Host ""
        Write-Host "🎉 Caesar icon ready for Windows integration!" -ForegroundColor Green
        Write-Host "  Icon file: $IcoPath" -ForegroundColor Cyan
        Write-Host "  Sizes: 16x16, 24x24, 32x32, 48x48, 64x64, 128x128, 256x256" -ForegroundColor Cyan
    }
} else {
    Write-Error "PNG icon not found: $PngPath"
}